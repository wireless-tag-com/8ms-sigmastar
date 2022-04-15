#include "qmsd_ui_entry.h"
#include "qmsd_internal_ui_cb.h"
#include <stdlib.h>
#include "lv_gif.h"

static lv_font_t ft_font_32;

lv_obj_t* screen_main;
lv_obj_t* screen_main_label_1;
lv_obj_t* screen_main_button_1;
lv_obj_t* screen_main_button_1_label;
lv_obj_t* screen_main_play_list;
void refresh_play_list(lv_obj_t* play_list)
{
    lv_list_clean(play_list);
    char c[100];
	FILE *fp;
	fp = popen("ls /etc/*.flv /etc/*.mp4 /etc/*.avi", "r");
	int i = 0;
	while (!feof(fp))
	{
		if (fgets(c, sizeof(c), fp))
        {
            if(strrchr(c,'/'))
            {
                char* bk=strrchr(c,'\n');
                *bk='\0';
                lv_obj_t* obj=lv_list_add_btn(play_list,LV_SYMBOL_VIDEO,strrchr(c,'/')+1);
                lv_obj_set_style_local_bg_color(obj,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
                lv_obj_set_style_local_text_color(obj,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_WHITE);
                lv_obj_set_style_local_bg_color(obj,LV_BTN_PART_MAIN,LV_STATE_PRESSED,LV_COLOR_WHITE);
                lv_obj_set_style_local_text_color(obj,LV_BTN_PART_MAIN,LV_STATE_PRESSED,LV_COLOR_BLACK);
                lv_obj_t* label = lv_list_get_btn_label(obj);
                lv_label_set_long_mode(label,LV_LABEL_LONG_EXPAND);
                lv_obj_set_event_cb(obj,__qmsd_play_list_button_cb);
            }
        }
		i++;
	}
	pclose(fp);
}

static void screen_main_play_list_create(void)
{
    screen_main_play_list=lv_list_create(screen_main,NULL);
    lv_obj_set_size(screen_main_play_list,512,600);
    lv_obj_set_pos(screen_main_play_list,1024,0);
    lv_obj_set_style_local_bg_color(screen_main_play_list,LV_LIST_PART_BG,LV_STATE_DEFAULT,LV_COLOR_BLACK);
    lv_obj_set_style_local_bg_color(screen_main_play_list,LV_LIST_PART_SCROLLABLE,LV_STATE_DEFAULT,LV_COLOR_BLACK);
    refresh_play_list(screen_main_play_list);
    //lv_obj_set_hidden(screen_main_play_list,true);
}

static int screen_main_button_1_create(void)
{
    screen_main_button_1 = lv_btn_create(screen_main, NULL);
    if (screen_main_button_1) {
        lv_obj_set_size(screen_main_button_1,60,320);
        lv_obj_set_style_local_radius(screen_main_button_1,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,0);
        screen_main_button_1_label = lv_label_create(screen_main_button_1, NULL);
        if (screen_main_button_1_label) {
            lv_label_set_text(screen_main_button_1_label, qmsd_lang_get_trans("main_button_1", "\n播\n放\n列\n表\n"));
            lv_obj_set_style_local_text_font(screen_main_button_1_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&ft_font_32);
        } else {
            lv_obj_del(screen_main_button_1);
            screen_main_button_1 = NULL;
            return -1;
        }

        lv_obj_align(screen_main_button_1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
        lv_obj_set_event_cb(screen_main_button_1, __qmsd_main_button_1_cb);
        lv_obj_align(screen_main_button_1,screen_main,LV_ALIGN_IN_RIGHT_MID,0,0);

        return 0;
    }

    return -1;
}

static int screen_main_label_1_create(void)
{
    screen_main_label_1 = lv_label_create(screen_main, NULL);
    if (screen_main_label_1) {
        lv_obj_set_style_local_text_font(screen_main_label_1,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&ft_font_32);
        lv_label_set_text(screen_main_label_1, qmsd_lang_get_trans("screen_main_label_1", "8MS平台展示DEMO"));
        lv_label_set_long_mode(screen_main_label_1,LV_LABEL_LONG_SROLL_CIRC);
        lv_obj_align(screen_main_label_1, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
        return 0;
    }

    return -1;
}

static void screen_main_qmsd_cb(lv_obj_t * obj, lv_event_t event, void *data)
{
    if (event == LV_EVENT_SCREEN_LOAD) {
        if (screen_main_button_1_label) {
        }
    }
}
lv_obj_t* video_page_create(lv_obj_t* par,int x,int y,int w,int h)
{
    lv_obj_t* video_page=lv_page_create(par?par:lv_scr_act(),NULL);
    lv_obj_set_size(video_page,w,h);
    lv_obj_set_pos(video_page,x,y);
    lv_obj_set_style_local_bg_color(video_page,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_MAKE(0x00,0x80,0x00));

    return video_page;
}
int screen_main_build(void)
{
    lv_freetype_init(64);
    lv_freetype_font_init(&ft_font_32, "/etc/8ms.ttf", 32);
    screen_main = lv_obj_create(NULL, NULL);
    if (screen_main) {
        lv_obj_set_style_local_bg_color(screen_main,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
        lv_obj_t* video_page = video_page_create(screen_main,0,0,600,600);
        qmsd_enable_vedio_stream(0x00, 0x80, 0x00, 1);
        screen_main_play_list_create();
        screen_main_button_1_create();

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