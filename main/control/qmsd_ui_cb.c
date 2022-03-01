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
        if (!strcmp(name, "test_button_1")) {
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
        } else if (!strcmp(name, "main_button_1")) {
            cJSON *attr;

            attr = cJSON_CreateObject();

            if (attr) {
                cJSON_AddStringToObject(attr, "action", "switch");
                qmsd_ctrl_cjson_gui("test", QMSD_CTRL_SET_STATUS, attr);

                cJSON_Delete(attr);
            }

            attr = cJSON_CreateObject();

            if (attr) {
                char *j_str;

                cJSON_AddStringToObject(attr, "name", "test_button_1");
                cJSON_AddStringToObject(attr, "action", "click");

                j_str = cJSON_PrintUnformatted(attr);
                qmsd_main_msgque_send(j_str, strlen(j_str));
                qmsd_gui_notify();
                cJSON_Delete(attr);
            }    
        }
    }
}
