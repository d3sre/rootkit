# Find Source files
SOURCE_FILES = $(shell ls $(PWD)/*.c)
# create object files
OBJECTS = $patsubst $(PWD)/%.c,%.o,$(SOURCE_FILES))

Obj-m += rooty.o
$(PROJECT)-objs = $(OBJECTS)

KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD)
clean:
	rm -rf *.o *.ko *.symvers *.mod.* *.order