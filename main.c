#include "rainbow.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Pawel Boczarski");
MODULE_DESCRIPTION("Simple module for driving a gpio-connected RGB led.");

#define RAINBOW_MAJOR	0
#define RAINBOW_MINOR	15

// how much colors/transition may be defined


struct rainbow_dev *dev;

int chrdevok = 0;



struct file_operations rainbow_fops = {
	.owner = THIS_MODULE,
	.read = rainbow_read,
	.write = rainbow_write,
	.open = NULL,
	.release = NULL
};

static int __init _initialize(void) {

	dev_t my_dev;
//	struct cdev *my_cdev;

	dev = kmalloc(sizeof(struct rainbow_dev), GFP_KERNEL);
	memset(dev, 0, sizeof(struct rainbow_dev));

	dev->currentState.R = 0;
	dev->currentState.G = 0;
	dev->currentState.B = 0;
	dev->nrdefs = 0;
	dev->nrdefs_max = RAINBOW_CONFIGS_NUM;
	dev->lastWrittenCommand = kmalloc(RAINBOW_DATABUFFER_LENGTH, GFP_KERNEL);

	dev->rainbow_configs = kmalloc(sizeof(rainbow_configdef) * dev->nrdefs_max, GFP_KERNEL);

	sema_init(&(dev->sem), 1);

	chrdevok = 0; // not ok
	//*name = 0;
	
	printk(KERN_ALERT "Hello driver!\n");
	if(alloc_chrdev_region(&my_dev, RAINBOW_MINOR, 1, "hello") != 0) {
		printk(KERN_ALERT "There was error allocating char device\n");
		goto error;
	} else {
		printk(KERN_ALERT "Char device is fine, device %d:%d registered", MAJOR(my_dev), MINOR(my_dev));
	}
	
//	init_configs();

	//my_cdev = cdev_alloc();			// change to cdev_init
	cdev_init(&(dev->cdev), &rainbow_fops);
	dev->cdev.ops = &rainbow_fops;
	{
		cdev_add(&(dev->cdev), my_dev, 1);
	}

	{
		rainbow_configdef c;

	//	printk(KERN_ALERT "adding configdefs...");
		TRACE((TRACE_LEVEL "adding configdefs..."));

		add_configdef("muteall", configdef(putdown, putdown, putdown));
		add_configdef("red", configdef(lightup, putdown, putdown));
		add_configdef("green", configdef(putdown, lightup, putdown));
		add_configdef("blue", configdef(putdown, putdown, lightup));
	}
	
	drv_hw_open();

	return 0;
error:
	return -1;
}

static void __exit _exit(void) {
//	cdev_del(dev->cdev);

	if(chrdevok) {
		unregister_chrdev_region(dev, 1);
	}
	
	drv_hw_close();

	printk(KERN_ALERT "Goodbye, cruel driver!\n");
}

module_init(_initialize);
module_exit(_exit);
