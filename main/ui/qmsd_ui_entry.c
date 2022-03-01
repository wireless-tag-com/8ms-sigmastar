#include "qmsd_ui_entry.h"
#include "qmsd_ctrl.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    lv_obj_t **id;
} lv_widget_t;

typedef struct
{
    char *name;
    lv_obj_t **id;
    lv_widget_t *wids;
    int len;
} lv_screen_t;

typedef struct
{
    char *name;
    lv_img_src_t* img;
} qmsd_img_t;

/*
* includes
*/

/*
* declear screens
*/



lv_widget_t g_screen_main_wids[] =
{
    {"main_button_1",&screen_main_button_1},
};

lv_widget_t g_screen_test_wids[] =
{
    {"test_button_1",&screen_test_button_1},
};

lv_screen_t g_screens[] =
{
    {"main", &screen_main, g_screen_main_wids, (sizeof(g_screen_main_wids) / sizeof(g_screen_main_wids[0]))},
    {"test", &screen_test, g_screen_test_wids, (sizeof(g_screen_test_wids) / sizeof(g_screen_test_wids[0]))},
};

qmsd_img_t g_qmsd_img[] = 
{
    {"clock_bg",&img1639365041009_png},
};

void qmsd_ui_entry(void)
{
    LV_FONT_DECLARE(ali_font_16);
    qmsd_set_screen(qmsd_get_screen);
    qmsd_set_widget(qmsd_get_widget);
    qmsd_set_img(qmsd_get_img);

    screen_main_build();
    screen_test_build();
    
    lv_scr_load(screen_main);
}

lv_obj_t *qmsd_get_screen(const char *name)
{
    int i = 0;
    for (i = 0; i < (sizeof(g_screens) / sizeof(g_screens[0])); i++)
    {
        if (!strcmp(name, g_screens[i].name))
        {
            return *(g_screens[i].id);
        }
    }
    return NULL;
}

lv_obj_t *qmsd_get_widget(const char *w_name)
{
    int i = 0, j = 0;

    for (i = 0; i < (sizeof(g_screens) / sizeof(g_screens[0])); i++)
    {
        for (j = 0; j < g_screens[i].len; j++)
        {
            if (!strcmp(w_name, g_screens[i].wids[j].name))
            {
                if (*(g_screens[i].id)) {
                    return *(g_screens[i].wids[j].id);
                } else {
                    return NULL;
                }
            }
        }
    }

    return NULL;
}

const char *qmsd_get_obj_name(lv_obj_t *obj)
{
    if (!obj) {
        return NULL;
    }

    int i = 0, j = 0;
    for (i = 0; i < (sizeof(g_screens) / sizeof(g_screens[0])); i++)
    {
        if (*(g_screens[i].id) == obj) {
            return g_screens[i].name;
        }

        for (j = 0; j <  g_screens[i].len; j++)
        {
            if (*(g_screens[i].wids[j].id) == obj)
            {
                return g_screens[i].wids[j].name;
            }
        }
    }

    return NULL;
}

lv_img_src_t *qmsd_get_img(const char *w_name)
{
    int i = 0;

    for (i = 0; i < (sizeof(g_qmsd_img) / sizeof(g_qmsd_img[0])); i++)
    {
        if (!strcmp(w_name, g_qmsd_img[i].name))
        {
            if (g_qmsd_img[i].img) {
                return g_qmsd_img[i].img;
            } else {
                return NULL;
            }
        }
    }

    return NULL;
}
