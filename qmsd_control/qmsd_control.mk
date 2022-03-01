CLSRCS += $(shell find -L $(TOPDIR)/qmsd_control/control -name \*.c -exec basename {} \;)
CLSRCS += $(shell find -L $(TOPDIR)/qmsd_control -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/qmsd_control/control $(TOPDIR)/qmsd_control
VPATH += :$(TOPDIR)/qmsd_control $(TOPDIR)/qmsd_control/control
CFLAGS += "-I$(TOPDIR)/qmsd_control " "-I$(TOPDIR)/qmsd_control/control"

