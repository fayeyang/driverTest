ifneq ($(KERNELRELEASE),)
#aaobjs:=$(subst $(usrPwd),,$(aaobjs))
#aaobjs:=$(notdir $(aaobjs))
obj-m:=busModule.o
busModule-objs:=$(usrObjs)
else
#usrPwd:=/home/faye/work/work_normal/gcc_test/device_driver_test
usrSrc:=$(wildcard *.c)
usrSrc:=$(patsubst %.mod.c,,$(usrSrc))
usrObjs:=$(usrSrc:.c=.o)
usrSrc:=$(wildcard *.cpp)
usrSrc:=$(patsubst %.mod.cpp,,$(usrSrc))
usrObjs+=$(usrSrc:.cpp=.o)
export usrObjs
KERNELDIR?=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

all:clean
	$(MAKE) -C device_test/
	$(MAKE) -C driver_test/
	@echo objs=$(usrObjs)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

.PHONY:clean
clean:
	@echo objs=$(usrObjs)
	rm -rf *.o *.ko *.mod.c *.mod.o .tmp_versions/ .*.cmd *.order *.symvers
endif




