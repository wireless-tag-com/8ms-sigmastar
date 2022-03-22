#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libubox/uloop.h>

#include "lvgl.h"
#include "qmsd_ctrl.h"
#include "qmsd_msgque.h"
#include "qmsd_control.h"
#include "qmsd_ui_cb.h"

#include "qmsd_api.h"
#include "qmsd_ubus.h"

#define QMSD_CONTORL_TIMEOUT    20  /* 20ms */

struct uloop_fd g_control_ufd;
static struct uloop_timeout g_control_timer;

static void __qmsd_control_loop_cb(struct uloop_fd *u, unsigned int events)
{
    if (events & ULOOP_READ) {
        int msg_num = 1;
        int ret;

        ret = read(u->fd, &msg_num, sizeof(msg_num));

        if (ret > 0) {
            void *msg;
            unsigned int msglen;
            int ret_msg;

            ret_msg = qmsd_main_msgque_recv((void **)&msg, &msglen);
            if (ret_msg == 0) {
                qmsd_ubus_send_event(msg);
                /*
                 * handle gui cb msg
                 */

                printf("msg: %s\n", msg);

                free(msg);
            }
        }
    }
}

static void __qmsd_control_timer_cb(struct uloop_timeout *timeout)
{
    /*
     * time task handler
     */
}

static void __qmsd_event_cb(const char *event, char *msg)
{

}

void qmsd_control_init(int *pipe_fd)
{
    qmsd_set_ui_event_cb(qmsd_ui_cb);

    uloop_init();

    if (pipe_fd) {
        g_control_ufd.fd = *pipe_fd;
        g_control_ufd.cb = __qmsd_control_loop_cb;
        uloop_fd_add(&g_control_ufd, ULOOP_READ);
    }

    g_control_timer.cb = __qmsd_control_timer_cb;
    uloop_timeout_set(&g_control_timer, QMSD_CONTORL_TIMEOUT);

    qmsd_ubus_init();
    qmsd_ubus_set_event_cb(__qmsd_event_cb);

    uloop_run();

    uloop_done();
}

void qmsd_control_deinit(void)
{
    qmsd_ubus_clean();
    uloop_done();
}

