MSRCS += $(shell find -L $(TOPDIR)/main -name \*.c -exec basename {} \;)
 
DEPPATH += --dep-path $(TOPDIR)/main
VPATH += :$(TOPDIR)/main $(TOPDIR)/main/control
CFLAGS += "-I$(TOPDIR)/main"
