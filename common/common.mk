CSRCS += $(shell find -L $(TOPDIR)/common -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/common
VPATH += :$(TOPDIR)/common
CFLAGS += "-I$(TOPDIR)/common"

