CC := arm-linux-gnueabihf-gcc
STRIP := arm-linux-gnueabihf-strip
CFLAGS := -Os -mno-unaligned-access -g -I$(LVGL_DIR)/ -DFBOSD_SUPPORT -DSSD21X

CSRCS += $(shell find -L $(TOPDIR)/ssd21x -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/ssd21x
VPATH += :$(TOPDIR)/ssd21x
CFLAGS += "-I$(TOPDIR)/ssd21x"
