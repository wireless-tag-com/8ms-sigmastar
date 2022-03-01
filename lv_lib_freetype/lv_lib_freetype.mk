FTSRCS += $(shell find -L $(TOPDIR)/lv_lib_freetype -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/lv_lib_freetype
VPATH += :$(TOPDIR)/lv_lib_freetype

CFLAGS += "-I$(TOPDIR)/lv_lib_freetype"