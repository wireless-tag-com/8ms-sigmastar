#include "qmsd_ui_entry.h"
#include "qmsd_internal_ui_cb.h"
#include <stdlib.h>
#include "lv_gif.h"

lv_obj_t* screen_main;
lv_obj_t* screen_main_label_1;
lv_obj_t* screen_main_button_1;
lv_obj_t* screen_main_button_1_label;

static int screen_main_button_1_create(void)
{
    screen_main_button_1 = lv_btn_create(screen_main, NULL);
    if (screen_main_button_1) {
        screen_main_button_1_label = lv_label_create(screen_main_button_1, NULL);
        if (screen_main_button_1_label) {
            lv_label_set_text(screen_main_button_1_label, qmsd_lang_get_trans("main_button_1", "BTNM"));
        } else {
            lv_obj_del(screen_main_button_1);
            screen_main_button_1 = NULL;
            return -1;
        }

        lv_obj_align(screen_main_button_1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
        lv_obj_set_event_cb(screen_main_button_1, __qmsd_main_button_1_cb);

        return 0;
    }

    return -1;
}

static int screen_main_label_1_create(void)
{
    screen_main_label_1 = lv_label_create(screen_main, NULL);
    if (screen_main_label_1) {
        lv_obj_align(screen_main_label_1, NULL, LV_ALIGN_CENTER, 0, 0);
        return 0;
    }

    return -1;
}

static void screen_main_qmsd_cb(lv_obj_t * obj, lv_event_t event, void *data)
{
    if (event == LV_EVENT_SCREEN_LOAD) {
        if (screen_main_button_1_label) {
            lv_label_set_text(screen_main_button_1_label, qmsd_lang_get_trans("main_button_1", "BTNM"));
        }
    }
}

int screen_main_build(void)
{
    screen_main = lv_obj_create(NULL, NULL);
    if (screen_main) {
        lv_obj_set_style_local_bg_color(screen_main,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);

        screen_main_button_1_create();
        screen_main_label_1_create();

        lv_obj_qmsd_set_cb(screen_main, screen_main_qmsd_cb);
        qmsd_call_ui_event_cb("main", screen_main, LV_EVENT_SCREEN_CREATE);
    }

    return -1;
}

void screen_main_delete(void)
{
    if (screen_main) {
        lv_obj_del(screen_main);
    }
}