/*
 * state.c
 *
 *  Created on: 2012-08-15
 *      Author: pawel
 */

#include "rainbow.h"

void apply_configdef(struct rainbow_dev *dev, char *name) {
	rainbow_configdef c = find_configdef(dev->lastWrittenCommand);
	rainbow_diodestate statenew;
	statenew = dev->currentState;

	switch(c.R) {
	case lightup:
		statenew.R = 1; break;
	case putdown:
		statenew.R = 0; break;
	case toggle:
		statenew.R = (c.R == 0 ? 1 :  0); break;
	case leave: break;
	}

	switch(c.G) {
	case lightup:
		statenew.G = 1; break;
	case putdown:
		statenew.G = 0; break;
	case toggle:
		statenew.G = (c.G == 0 ? 1 :  0); break;
	case leave: break;
	}

	switch(c.B) {
	case lightup:
		statenew.B = 1; break;
	case putdown:
		statenew.B = 0; break;
	case toggle:
		statenew.B = (c.B == 0 ? 1 :  0); break;
	case leave: break;
	}

	drv_setDiodeState(statenew);
	dev->currentState = statenew;
}
