#include "qmsd_ui_entry.h"
extern void refresh_play_list(lv_obj_t* play_list);
void __qmsd_main_button_1_cb(lv_obj_t* obj,lv_event_t event)
{
    if (event==LV_EVENT_CLICKED)
    {
        static bool set_up=false;
        if (!set_up)
        {
            refresh_play_list(screen_main_play_list);
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
            lv_anim_set_var(&a, screen_main_play_list);
            lv_anim_set_time(&a,200);
            lv_anim_set_values(&a, 1024,600);
            lv_anim_start(&a);
            lv_anim_t b;
            lv_anim_init(&b);
            lv_anim_set_exec_cb(&b, (lv_anim_exec_xcb_t)lv_obj_set_x);
            lv_anim_set_var(&b, screen_main_button_1);
            lv_anim_set_time(&b,200);
            lv_anim_set_values(&b, 964,540);
            lv_anim_start(&b);  
            lv_label_set_text(screen_main_button_1_label,"\n返\n\n回\n");
        }
        else
        {
            //refresh_play_list(screen_main_play_list);
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
            lv_anim_set_var(&a, screen_main_play_list);
            lv_anim_set_time(&a,200);
            lv_anim_set_values(&a,600, 1024);
            lv_anim_start(&a);
            lv_anim_t b;
            lv_anim_init(&b);
            lv_anim_set_exec_cb(&b, (lv_anim_exec_xcb_t)lv_obj_set_x);
            lv_anim_set_var(&b, screen_main_button_1);
            lv_anim_set_time(&b,200);
            lv_anim_set_values(&b,540 ,964);
            lv_anim_start(&b);  
            
            lv_label_set_text(screen_main_button_1_label, qmsd_lang_get_trans("main_button_1", "\n播\n放\n列\n表\n"));
        }
        set_up=!set_up;
    }

    qmsd_call_ui_event_cb("main_button_1", obj, event);
}

void __qmsd_test_button_1_cb(lv_obj_t* obj,lv_event_t event)
{
    if (event==LV_EVENT_CLICKED)
    {
        printf("test btn1 pressed\n");
    }

    qmsd_call_ui_event_cb("test_button_1", obj, event);
}

void __qmsd_play_list_button_cb(lv_obj_t* obj,lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        lv_event_send(screen_main_button_1,LV_EVENT_CLICKED,NULL);
        qmsd_call_ui_event_cb("play_list_btn", obj, event);
    }
}