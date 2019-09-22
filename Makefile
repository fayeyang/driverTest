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
	@echo objs=$(usrObjs)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	$(MAKE) -C class_test/
	$(MAKE) -C device_test/
	$(MAKE) -C driver_test/

.PHONY:clean
clean:
	$(MAKE) -C device_test/ clean
	$(MAKE) -C driver_test/ clean
	$(MAKE) -C class_test/ clean
	@echo objs=$(usrObjs)
	# rm -rf *.o *.ko *.mod.c *.mod.o .tmp_versions/ .*.cmd *.order *.symvers .*.mk
	rm -rf *.o *.ko *.mod.c *.mod.o .tmp_versions/ .*.cmd *.order .*.mk
endif




