#
# qmsd Makefile
#
CHIP ?=ssd20x
MODE ?=NORMAL
UBUS ?=n

TOPDIR := ${shell pwd}
LVGL_DIR_NAME := lvgl
LVGL_DIR := ${shell pwd}

include $(CHIP)/config.mk

CFLAGS += -I$(LVGL_DIR)/
CFLAGS += -DLV_CONF_INCLUDE_SIMPLE -DLV_EX_CONF_INCLUDE_SIMPLE
CFLAGS += -I$(TOPDIR)/include/ -I$(TOPDIR)/include/lvgl/ -I$(TOPDIR)/lvgl/
CFLAGS += -I$(TOPDIR)/extra/include/$(CHIP)/
CFLAGS += -I$(TOPDIR)/extra/include/ -I$(TOPDIR)/extra/include/freetype2

LLDFLAGS := -L./lib/$(CHIP) -L$(TOPDIR)/extra/lib/$(CHIP) -lcrypto -lwtinfo -lcJSON -lm -lmi_sys -lmi_disp -lmi_common -lmi_panel -lmi_gfx -lpthread -ldl
MLDFLAGS := -L./lib/$(CHIP) -L$(TOPDIR)/extra/lib/$(CHIP) -llvgl -lcrypto -lwtinfo -lcJSON -llvgl -lm -lmi_sys -lmi_disp -lmi_common -lmi_panel -lmi_gfx -lpthread -ldl

ifeq ($(CHIP),ssd21x)
LLDFLAGS += -lmi_fb
MLDFLAGS += -lmi_fb -lcam_os_wrapper
else
MLDFLAGS += -lmi_vdec
endif

ifeq ($(MODE),fastboot)
CFLAGS += -DFASTBOOT
endif

ifeq ($(UBUS),y)
CFLAGS += -DSUPPORT_UBUS
endif

ifeq ($(UBUS),y)
UBUSLIBS += -lubus -lblobmsg_json -ljson-c
endif

MSRCS += $(shell find . -maxdepth 1 -name \*.c)

include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/lv_lib_freetype/lv_lib_freetype.mk
include $(LVGL_DIR)/common/common.mk
include $(LVGL_DIR)/qmsd_ui/qmsd_ui.mk
include $(LVGL_DIR)/qmsd_control/qmsd_control.mk
include $(LVGL_DIR)/main/qmsd.mk

OBJEXT ?= .o
UIOBJEXT ?= .uo
CLOBJEXT ?= .co
MOBJS = $(MSRCS:.c=$(OBJEXT))
UIOBJS = $(UISRCS:.c=$(UIOBJEXT))
CLOBJS = $(CLSRCS:.c=$(CLOBJEXT))

ifeq ($(MODE),fastboot)
TARGETS = libqmsdui.so libqmsdcl.so
else
MLDFLAGS += -lbz2 -lz -lpng -lfreetype -llvfreetype -lubox -lmi_divp -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common $(UBUSLIBS)
endif

all: clean prepare $(TARGETS) qmsd-demo

prepare:
	@mkdir -p build build_ui build_cl bin lib/$(CHIP)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o build/$@ $^
	@echo "CC $^"

%.uo: %.c
	@$(CC) -fpic $(CFLAGS) -c -o build_ui/$@ $^
	@echo "CC $^"

%.co: %.c
	@$(CC) -fpic $(CFLAGS) -c -o build_cl/$@ $^
	@echo "CC $^"

libqmsdui.so: $(UIOBJS)
	$(CC) -shared -o lib/$(CHIP)/libqmsdui.so build_ui/*.uo $(MLDFLAGS) -lbz2 -lz -lpng -lfreetype -llvfreetype
	$(STRIP) --strip-all lib/$(CHIP)/libqmsdui.so

libqmsdcl.so: $(CLOBJS)
	$(CC) -shared -o lib/$(CHIP)/libqmsdcl.so build_cl/*.co $(MLDFLAGS) -lubox 
	$(STRIP) --strip-all lib/$(CHIP)/libqmsdcl.so

qmsd-demo: $(MOBJS)
	$(CC) -o bin/$@ build/*.o $(MLDFLAGS)
	cp bin/$@ bin/$@_debug
	$(STRIP) --strip-all bin/$@

clean:
	@rm -rf build bin build_ui build_cl lib
