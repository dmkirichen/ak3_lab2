ifneq ($(KERNELRELEASE),)
#kbuild part of makefile
obj-m := hello.o
ccflags-y += -g
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello.ko hello.ko.unstrippped
	$(CROSS_COMPILE)strip -g hello.ko  # strip only debugging info
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	%.s %.i: %.c  # using make hello.s instead of objdump
	$(MAKE) -C $(KDIR) M=$$PWD $@
endif
