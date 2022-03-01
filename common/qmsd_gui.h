#ifndef __QMSD_GUI_H__
#define __QMSD_GUI_H__

#define __need_timeval 

#include "lvgl.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*qmsd_ui_init)(void);

typedef void (*qmsd_ui_event_cb)(const char *obj_name, lv_obj_t* obj,lv_event_t event);
void qmsd_set_ui_event_cb(qmsd_ui_event_cb init_cb);
void qmsd_call_ui_event_cb(const char *obj_name, lv_obj_t* obj,lv_event_t event);

int qmsd_gui_init(qmsd_ui_init ui_init, int *pipe_fd, bool fastboot);
void qmsd_gui_deinit(void);
int qmsd_gui_lock(void);
int qmsd_gui_unlock(void);
int qmsd_gui_notify(void);

#ifdef __cplusplus
}
#endif

#endif
