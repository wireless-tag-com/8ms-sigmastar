#ifdef SUPPORT_UBUS
#include <stdio.h>
#include "qmsd_ubus.h"
#include <libubox/list.h>
#include <libubox/uloop.h>
#include <libubox/ustream.h>

#include <libubox/blobmsg_json.h>
#include <libubus.h>

static struct ubus_context *g_ubus_ctx;
static struct blob_buf g_bbuf;
static struct ubus_event_handler g_qmsd_listener;
static struct blob_buf g_event_buf;
static qmsd_ubus_event_cb g_qmsd_ubus_event_cb;

static void __qmsd_ubus_reconnect_timer(struct uloop_timeout *timeout)
{
    static struct uloop_timeout retry = {
        .cb = __qmsd_ubus_reconnect_timer,
    };
    int t = 2;

    if (ubus_reconnect(g_ubus_ctx, NULL) != 0) {
        uloop_timeout_set(&retry, t * 1000);
        return;
    }

    ubus_add_uloop(g_ubus_ctx);
}

static void __qmsd_ubus_connection_lost(struct ubus_context *ctx)
{
    __qmsd_ubus_reconnect_timer(NULL);
}

static void __qmsd_ubus_event_cb(struct ubus_context *ctx, struct ubus_event_handler *ev,
              const char *type, struct blob_attr *msg)
{
    char *str;

    str = blobmsg_format_json_indent(msg, true, 0);
    if (str) {
        if (g_qmsd_ubus_event_cb)
            g_qmsd_ubus_event_cb(type, str);
        free(str);
    }
}

static int qmsd_ubus_ctrl(struct ubus_context *ctx, struct ubus_object *obj,
    struct ubus_request_data *req, const char *method, struct blob_attr *msg)
{
    char *str;
    char *res;

    blob_buf_init(&g_bbuf, 0);

    str = blobmsg_format_json(msg, true);

    if (str) {
        res = qmsd_ctrl_str_gui(str);
        if (res) {
            blobmsg_add_json_from_string(&g_bbuf, res);
            free(res);
        }
    }

    ubus_send_reply(ctx, req, g_bbuf.head);

    return 0;
}

static const struct ubus_method qmsd_ubus_methods[] = {
    { .name = "ctrl", .handler = qmsd_ubus_ctrl },
};

static struct ubus_object_type qmsd_ubus_object_type =
    UBUS_OBJECT_TYPE("qmsd", qmsd_ubus_methods);

static struct ubus_object qmsd_ubus_object = {
    .name = "qmsd",
    .type = &qmsd_ubus_object_type,
    .methods = qmsd_ubus_methods,
    .n_methods = ARRAY_SIZE(qmsd_ubus_methods),
};

int qmsd_ubus_init(void)
{
    int ret;

    g_ubus_ctx = ubus_connect(NULL);
    if (!g_ubus_ctx) {
        printf("Failed to connect to ubus\n");
        return -1;
    }

    g_ubus_ctx->connection_lost = __qmsd_ubus_connection_lost;
    ubus_add_uloop(g_ubus_ctx);

    memset(&g_qmsd_listener, 0, sizeof(g_qmsd_listener));
    g_qmsd_listener.cb = __qmsd_ubus_event_cb;
    ret = ubus_register_event_handler(g_ubus_ctx, &g_qmsd_listener, QMSD_EVENT_TYPE);
    if (ret) {
        printf("listen %s failed\n", QMSD_EVENT_TYPE);
    }

    ubus_add_object(g_ubus_ctx, &qmsd_ubus_object);

    return ret;
}

void qmsd_ubus_clean(void)
{
    if (g_ubus_ctx) {
        ubus_free(g_ubus_ctx);
    }
}

static int __qmsd_ubus_call_api(const char *path, const char *api_name, const char *json_str, ubus_data_handler_t cb, int timeout)
{
    uint32_t id;
    int ret;

    if (!g_ubus_ctx)
        return -1;

    blob_buf_init(&g_bbuf, 0);

    if (json_str) {
        if (!blobmsg_add_json_from_string(&g_bbuf, json_str)) {
                fprintf(stderr, "Failed to parse message data\n");
            return -1;
        }
    }

    ret = ubus_lookup_id(g_ubus_ctx, path, &id);
    if (ret)
        return ret;

    return ubus_invoke(g_ubus_ctx, id, api_name, g_bbuf.head, cb, NULL, timeout * 1000);
}

extern int qmsd_api_call_cb(char *msg);

static void __qmsd_ubus_api_callback(struct ubus_request *req, int type, struct blob_attr *msg)
{
    char *str;

    if (!msg)
        return;

    str = blobmsg_format_json_indent(msg, true, 0);
    if (str) {
        qmsd_api_call_cb(str);
        free(str);
    }
}

int qmsd_ubus_call_api(const char *api_path, const char *api_name, const char *param)
{
    return __qmsd_ubus_call_api(api_path, api_name, param, __qmsd_ubus_api_callback, 30);
}

void qmsd_ubus_set_event_cb(qmsd_ubus_event_cb cb)
{
    if (cb)
        g_qmsd_ubus_event_cb = cb;
}

static int __qmsd_ubus_send_json_event(const char *event, const char *json_str)
{
    if (!g_ubus_ctx)
        return -1;

    blob_buf_init(&g_event_buf, 0);

    if (!blobmsg_add_json_from_string(&g_event_buf, json_str)) {
        return -1;
    } else {
        return ubus_send_event(g_ubus_ctx, event, g_event_buf.head);
    }
}

int qmsd_ubus_send_event(const char *msg)
{
    return __qmsd_ubus_send_json_event(QMSD_UI_EVENT_TYPE, msg);
}
#else
#include <stdio.h>
#include "qmsd_ubus.h"

void qmsd_ubus_set_event_cb(qmsd_ubus_event_cb cb)
{
}

int qmsd_ubus_call_api(const char *api_path, const char *api_name, const char *param)
{
	return -1;
}

int qmsd_ubus_send_event(const char *msg)
{
	return -1;
}

int qmsd_ubus_init(void)
{
	return -1;
}

void qmsd_ubus_clean(void)
{
}
#endif
