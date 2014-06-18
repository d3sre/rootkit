
# Reference to rooty.c file
obj-m += rooty.o

# Path to Kernel Files
KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
# All options (automatically selected)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD)
# Clean to clean up again the files	
clean:
	rm -rf *.o *.ko *.symvers *.mod.* *.order