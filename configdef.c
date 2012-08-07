/*
 * configdef.c
 *
 *  Created on: 2012-02-28
 *      Author: pawel
 */

#include "rainbow.h"

// unsafe!
int configdef_strcmp(char *str1, char *str2) {
	int i = 0;

	while(++i < RAINBOW_CONFIGDEF_NAME_MAX_LEN
			&& str1 != 0 && str1 != 13 && str1 != 10
			&& str2 != 0 && str2 != 13 && str2 != 10
			&& *str1++ == *str2++);

	if((*str1 == 0 || *str1 == 13 || *str1 == 10)
			&& (*str2 == 0 || *str2 == 13 || *str2 == 10)) {
		return 0;
	} else if(*str1 == 0 || *str1 == 13 || *str1 == 10) {
		return -*str2;
	} else if(*str2 == 0 || *str2 == 13 || *str2 == 10) {
		return *str1;
	} else if(i >= RAINBOW_CONFIGDEF_NAME_MAX_LEN) {
		return -1;
	} else if(*str1 != *str2) {
		return *str1 - *str2;
	}
}

rainbow_configdef configdef(int R, int G, int B) {
	rainbow_configdef def;

	def.R = R;
	def.G = G;
	def.B = B;

	return def;
}

void add_configdef(char *name, rainbow_configdef cdef) {
	if(dev->nrdefs < dev->nrdefs_max) {
		strncpy(dev->rainbow_configs[dev->nrdefs].name, name, RAINBOW_CONFIGDEF_NAME_MAX_LEN);
		dev->rainbow_configs[dev->nrdefs].configdef = cdef;

		dev->nrdefs++;
	} else {
		printk(KERN_ALERT "No place for next colordef!");
	}
}

rainbow_configdef find_configdef(char *name) {
	printk(KERN_ALERT "Looking for configdef %s...", name);
	int i = 0;

	while(configdef_strcmp(dev->rainbow_configs[i].name, name) != 0) {
		i++;
		if(i >= dev->nrdefs_max) {
			goto err_op;
		}
	}

	TRACE((TRACE_LEVEL "Found configdef %s at position %d\n", name, i));
	return dev->rainbow_configs[i].configdef;

	err_op: {
		rainbow_configdef def;
		def.R = leave;
		def.G = leave;
		def.B = leave;

		TRACE((TRACE_LEVEL "No colordef with such name!"));

		return def;
	}
}
