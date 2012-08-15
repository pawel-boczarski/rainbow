ifneq ($(KERNELRELEASE),)
	rainbow-objs := main.o fops.o drv_$(LOW_LEVEL_DRV).o configdef.o state.o
	obj-m := rainbow.o
else

ifeq ($(ARM),1)
#	export ARCH=arm
#	export CROSS_COMPILE=arm-none-linux-gnueabi-
#	KERNELDIR = /home/pawel/workspace-olimex/linux-2.6
#	export PATH=$PATH:$HOME/bin/codesourcery-armgcc-2009q1/bin/:$HOME/bin
#	$(SHELL) source arm-source-me
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
endif

ifndef LOW_LEVEL_DRV
	# a real RGB diode connected to gpio 80, 81, 82 pins
	export LOW_LEVEL_DRV=gpio
endif

	PWD = $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

