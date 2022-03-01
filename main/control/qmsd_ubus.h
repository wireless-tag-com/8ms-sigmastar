#ifndef __QMSD_UBUS_H
#define __QMSD_UBUS_H

#define QMSD_EVENT_TYPE "/local/qmsd/*"

#define QMSD_UI_EVENT_TYPE "/local/qmsdui"

typedef void (*qmsd_ubus_event_cb)(const char *event, char *msg);
void qmsd_ubus_set_event_cb(qmsd_ubus_event_cb cb);
int qmsd_ubus_call_api(const char *api_path, const char *api_name, const char *param);
int qmsd_ubus_send_event(const char *msg);
int qmsd_ubus_init(void);
void qmsd_ubus_clean(void);

#endif
