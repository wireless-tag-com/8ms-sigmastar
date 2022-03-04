#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "qmsd_gui.h"
#include "qmsd_ctrl.h"
#include "qmsd_msgque.h"

#ifndef FASTBOOT
#include "qmsd_control.h"
#include "qmsd_ui_entry.h"
#endif

#include "log.h"

extern char *optarg;

int g_pipe_fd[2];

static bool use_syslog = true;
static int log_level = DEFAULT_LOG_LEVEL;

#ifdef SSD20X
extern int ssd20x_panel_init(void);
extern int ssd20x_panel_deinit(void);
#endif

static void usage(const char *programname)
{
    fprintf(stderr, "Usage: %s [options]\n"
            "Options:\n"
            " -l <level>:   Log output level (default: %d)\n"                   
            " -S:           Use stderr instead of syslog for log messages\n"
            "\n", programname, DEFAULT_LOG_LEVEL);
    exit(1);
}

static void __qmsd_handle_signal(int signo)
{
#ifdef SSD20X
    ssd20x_panel_deinit();
#endif

    close(g_pipe_fd[0]);

    qmsd_main_msgque_deinit();

    qmsd_gui_deinit();

    qmsd_ctrl_deinit();

    syslog_close();

    exit(0);
}

static void __qmsd_setup_signals(void)
{
    struct sigaction s;

    memset(&s, 0, sizeof(s));
    s.sa_handler = __qmsd_handle_signal;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    sigaction(SIGTERM, &s, NULL);

    s.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &s, NULL);
}

extern void qmsd_ui_logo_entry(void);

int main(int argc, char *argv[])
{
    int op;

    while ((op = getopt(argc, argv, "l:S")) != -1) {
        switch (op) {
        case 'l':
            log_level = atoi(optarg);
            break;

        case 'S':
            use_syslog = false;
            break;

        default:
            usage(argv[0]);
            break;
        }
    }

    syslog_init("qmsd", use_syslog, log_level);
    __qmsd_setup_signals();

    if (pipe(g_pipe_fd) < 0) {
        syslog_message(L_CRIT, "create pipe error\n");
        return -1;
    }

    fcntl(g_pipe_fd[0], F_SETFL, O_NONBLOCK);

#ifdef SSD20X
    ssd20x_panel_init();
#endif

    qmsd_main_msgque_init(128);

#ifdef FASTBOOT
    qmsd_gui_init(qmsd_ui_logo_entry, &g_pipe_fd[1], true);
    qmsd_ctrl_init(&g_pipe_fd[0]);
#else
    qmsd_gui_init(qmsd_ui_entry, &g_pipe_fd[1], false);
    qmsd_control_init(&g_pipe_fd[0]);
#endif

#ifdef SSD20X
    ssd20x_panel_deinit();
#endif

    close(g_pipe_fd[0]);
    qmsd_main_msgque_deinit();

    qmsd_gui_deinit();

    qmsd_ctrl_deinit();

    syslog_close();

    return 0;
}
