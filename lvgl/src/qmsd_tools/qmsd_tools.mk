CSRCS += qmsd_tools.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/qmsd_tools
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/qmsd_tools

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/qmsd_tools"
