#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "lvgl.h"
#include "qmsd_ctrl.h"
#include "qmsd_ui_cb.h"
#include "qmsd_control.h"

void qmsd_ui_cb(const char *name, lv_obj_t* obj,lv_event_t event)
{
    if (event==LV_EVENT_CLICKED)
    {
        if (!strcmp(name, "play_list_btn")) {
            char path[64];
            sprintf(path,"/etc/%s",lv_list_get_btn_text(obj));
            mm_player_close();
            mmplayer_open(path);
        }
        else if (!strcmp(name, "test_button_1")) {
            cJSON *attr;

            attr = cJSON_CreateObject();

            if (attr) {
                cJSON_AddStringToObject(attr, "action", "switch");
                qmsd_ctrl_cjson_gui("main", QMSD_CTRL_SET_STATUS, attr);

                cJSON_Delete(attr);
            }

            attr = cJSON_CreateObject();

            if (attr) {
                char *j_str;

                cJSON_AddStringToObject(attr, "name", "main_button_1");
                cJSON_AddStringToObject(attr, "action", "click");

                j_str = cJSON_PrintUnformatted(attr);
                qmsd_main_msgque_send(j_str, strlen(j_str));
                qmsd_gui_notify();
                cJSON_Delete(attr);
            }
        } 
    }
}
