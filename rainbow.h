/*
 * rainbow.h
 *
 *  Created on: 2012-02-28
 *      Author: pawel
 */

#ifndef RAINBOW_H_
#define RAINBOW_H_

#define DRV_CONSOLE
//#define DRV_GPIO

// for stupid eclipse to have fun
//#define __KERNEL__

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include "drv.h"

// in project settings, this is set up as defined.
// it should not be defined in build in any case!
//#ifdef WHAT_ECLIPSE_CANT_SEE
//#include "linux/kernel.h"
//#endif


//#define max(x, y) ((x) > (y) ? (x) : (y))
//#define min(x, y) ((x) < (y) ? (x) : (y))

#define RAINBOW_CONFIGS_NUM 64
#define RAINBOW_DATABUFFER_LENGTH 64
#define RAINBOW_CONFIGDEF_NAME_MAX_LEN 16

#define TRACE_ON
#define TRACE_LEVEL KERN_ALERT

#ifdef TRACE_ON
#define TRACE(x) do { \
	printk x; \
} while(0)

#else
#define TRACE(x) do { } while(0)
#endif

typedef enum _rainbow_action {
	putdown = 0,
	lightup = 1,
	leave = 2,
	toggle = 3
} rainbow_action;

typedef struct _rainbow_configdef {
	rainbow_action		R;
	rainbow_action		G;
	rainbow_action		B;
} rainbow_configdef;

typedef struct _rainbow_diodestate {
	int R;
	int G;
	int B;
} rainbow_diodestate;

typedef struct _rainbow_config {
	char name[RAINBOW_CONFIGDEF_NAME_MAX_LEN];
	rainbow_configdef configdef;
} rainbow_config;

struct rainbow_dev {
	struct _rainbow_config *rainbow_configs;
	int nrdefs;
	int nrdefs_max;
	char *lastWrittenCommand;	// last command buffer
	rainbow_diodestate currentState;
	struct semaphore sem;
	struct cdev cdev;
};

extern struct rainbow_dev *dev;

// configdef management
rainbow_configdef configdef(int R, int G, int B);
void add_configdef(char *name, rainbow_configdef cdef);
rainbow_configdef find_configdef(char *name);

// low level drv ops
int drv_hw_open();
void drv_setDiodeState(rainbow_diodestate state);
int drv_hw_close();

// file operations
ssize_t rainbow_read(struct file *filp, char __user *data, size_t datalen, loff_t *pos);
ssize_t rainbow_write(struct file *filp, const char __user *data, size_t datalen, loff_t *pos);

#endif /* RAINBOW_H_ */
