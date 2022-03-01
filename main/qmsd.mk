MSRCS += $(shell find -L $(TOPDIR)/main -name \*.c -exec basename {} \;)
MSRCS += $(shell find -L $(TOPDIR)/main/control -name \*.c -exec basename {} \;)
MSRCS += $(shell find -L $(TOPDIR)/main/custom -name \*.c -exec basename {} \;)
MSRCS += $(shell find -L $(TOPDIR)/main/ui -name \*.c -exec basename {} \;)
MSRCS += $(shell find -L $(TOPDIR)/main/res -name \*.c -exec basename {} \;)
 
DEPPATH += --dep-path $(TOPDIR)/main $(TOPDIR)/main/control $(TOPDIR)/main/custom $(TOPDIR)/main/ui $(TOPDIR)/main/res
VPATH += :$(TOPDIR)/main $(TOPDIR)/main/control $(TOPDIR)/main/custom $(TOPDIR)/main/ui $(TOPDIR)/main/res
CFLAGS += "-I$(TOPDIR)/main" "-I$(TOPDIR)/main/control" "-I$(TOPDIR)/main/custom" "-I$(TOPDIR)/main/ui" "-I$(TOPDIR)/main/res"
