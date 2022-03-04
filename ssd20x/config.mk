CC := arm-linux-gnueabihf-gcc
STRIP := arm-linux-gnueabihf-strip
CFLAGS := -O2 -g -I$(LVGL_DIR)/ -DFBOSD_SUPPORT -DSSD20X

MSRCS += $(shell find -L $(TOPDIR)/ssd20x -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/ssd20x
VPATH += :$(TOPDIR)/ssd20x
CFLAGS += "-I$(TOPDIR)/ssd20x"
