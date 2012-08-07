/*
 * drv_gpio.c
 *
 *  Created on: 2012-02-28
 *      Author: pawel
 */


#include "rainbow.h"
//#ifdef DRV_GPIO
#include "linux/gpio.h"

//#define GPIO_R	80
//#define GPIO_G	81
//#define GPIO_B	82
#define GPIO_R 		AT91_PIN_PB16
#define GPIO_G 		AT91_PIN_PB17
#define GPIO_B		AT91_PIN_PB18

#define LIGHT_ON_ZERO_FADE_ON_ONE

#ifdef LIGHT_ON_ZERO_FADE_ON_ONE
#define invert_if_needed(x) (1-x)
#else
#define invert_if_needed(x) (x)
#endif


int drv_hw_open() {
	int err = 0;

	err |= gpio_request(GPIO_R, "GPIO_R");
	err |= gpio_request(GPIO_G, "GPIO_G");
	err |= gpio_request(GPIO_B, "GPIO_B");

	err |= gpio_direction_output(GPIO_R, invert_if_needed(1));
	err |= gpio_direction_output(GPIO_G, invert_if_needed(1));
	err |= gpio_direction_output(GPIO_B, invert_if_needed(1));

	if(err) {
		TRACE((TRACE_LEVEL "An error occurred opening GPIOs!"));
	}

	return err;
}

void drv_setDiodeState(rainbow_diodestate state) {

	gpio_set_value(GPIO_R, invert_if_needed(state.R));
	gpio_set_value(GPIO_G, invert_if_needed(state.G));
	gpio_set_value(GPIO_B, invert_if_needed(state.B));
}

int drv_hw_close() {
	int err = 0;

	err |= gpio_direction_output(GPIO_R, invert_if_needed(0));
	err |= gpio_direction_output(GPIO_G, invert_if_needed(0));
	err |= gpio_direction_output(GPIO_B, invert_if_needed(0));

	gpio_free(GPIO_R);
	gpio_free(GPIO_G);
	gpio_free(GPIO_B);

	if(err) {
		TRACE((TRACE_LEVEL "An error occurred closing gpios!"));
	}

	return err;
}

//#endif
