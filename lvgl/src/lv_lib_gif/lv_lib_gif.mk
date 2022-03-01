CSRCS += gifdec.c lv_gif.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_gif
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_gif

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_lib_gif"