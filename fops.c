/*
 * fops.c
 *
 *  Created on: 2012-08-04
 *      Author: pawel
 */

#include "rainbow.h"

ssize_t rainbow_write(struct file *filp, const char __user *data, size_t datalen, loff_t *pos) {

//	printk(KERN_INFO "Write fop requested, filp=%p, data=%p, datalen=%d, *pos=%d", filp, data, datalen, *pos);
	TRACE((TRACE_LEVEL "Write fop requested, filp=%p, data=%p, datalen=%d, *pos=%d", filp, data, datalen, *pos));

	int written = 0;
	int maxPossibleToWrite = 63 - *pos;
	int toWrite = min(datalen, maxPossibleToWrite);

	if(down_interruptible(&(dev->sem))) {
		TRACE((TRACE_LEVEL "Write fop: return -EINTR"));
		return -EINTR;
	}

	if(toWrite <= 0) {
//		printk(KERN_ALERT "write: end of file reached or error occurred (toWrite = %d)!", toWrite);
		TRACE((TRACE_LEVEL "write: end of file reached or error occurred (toWrite = %d)!", toWrite));
		up(&(dev->sem));
		return 0;
	}

	if(toWrite > 0) {
		if(copy_from_user(dev->lastWrittenCommand + *pos, data, toWrite)) {
			up(&(dev->sem));
			return -EFAULT;
		}
		written = toWrite;
		dev->lastWrittenCommand[*pos+toWrite] = '\0';
	}

	// apply configdef by name
	apply_configdef(dev, dev->lastWrittenCommand);

	*pos += written;
	up(&(dev->sem));
	TRACE((TRACE_LEVEL "write fop: return %d", written));
	return written;
}

ssize_t rainbow_read(struct file *filp, char __user *data, size_t datalen, loff_t *pos) {
	char state[] = "R-G-B-";
	int wholeDataLen = sizeof state / sizeof *state;
	int dataLenFromCurrentPos = (wholeDataLen - *pos) > 0 ? wholeDataLen - *pos : 0;
	int actualDataLen = 0;

	TRACE((TRACE_LEVEL "Read fop requested, filp=%p, data=%p, datalen=%d, *pos=%d", filp, data, datalen, *pos));

	if(down_interruptible(&(dev->sem))) {
		TRACE((TRACE_LEVEL "returning -EINTR"));
		return -EINTR;
	}

	if(dev->currentState.R == 1) {
		state[1] = '+';
	}

	if(dev->currentState.G == 1) {
		state[3] = '+';
	}

	if(dev->currentState.B == 1) {
		state[5] = '+';
	}

	{
		actualDataLen = min(dataLenFromCurrentPos, datalen);

		if(actualDataLen == 0) {
			goto rainbow_read_nothing_to_read;
		}

		if(actualDataLen < 0) {
			goto rainbow_read_error;
		}

		copy_to_user(data, state + *pos, actualDataLen);
	}

	*pos += actualDataLen;
	up(&(dev->sem));
	TRACE((TRACE_LEVEL "Returned: %d", actualDataLen));
	return actualDataLen;

	rainbow_read_nothing_to_read:
		printk(KERN_INFO "Nothing to read");
		up(&(dev->sem));
		return 0;

	rainbow_read_error:
		printk(KERN_ALERT "A read error occurred");
		up(&(dev->sem));
		return -EFAULT;

	return 0;
}
