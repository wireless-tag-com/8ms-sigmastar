MSRCS += $(shell find -L $(TOPDIR)/qmsd_ui/ui -name \*.c -exec basename {} \;)
MSRCS += $(shell find -L $(TOPDIR)/qmsd_ui/res -name \*.c -exec basename {} \;)

DEPPATH += --dep-path $(TOPDIR)/qmsd_ui/ui $(TOPDIR)/qmsd_ui/res
VPATH += :$(TOPDIR)/qmsd_ui/ui $(TOPDIR)/qmsd_ui/res
CFLAGS += "-I$(TOPDIR)/qmsd_ui/ui" "-I$(TOPDIR)/qmsd_ui/res"

