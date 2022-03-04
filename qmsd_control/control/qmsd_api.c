#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <libubus.h>
#include <libubox/list.h>

#include "qmsd_api.h"

#define QMSDAPI_PATH	"/usr/sbin/"
#define QMSDAPI_PROCESS_NAME	"qmsdapi"
#define QMSDAPI_QMSD_ARGV	"qmsd"
#define QMSDAPI_MAX_RECV_BUFFER		8192

static qmsd_api_cb g_qmsd_api_cb;

static struct list_head qmsd_api_process_list = LIST_HEAD_INIT(qmsd_api_process_list);

void qmsd_api_set_cb(qmsd_api_cb cb)
{
    if (cb) {
        g_qmsd_api_cb = cb;
    }
}

int qmsd_api_call_cb(char *msg)
{
    if (msg) {
        if (g_qmsd_api_cb) {
            g_qmsd_api_cb(msg);
        }
    }
}

static void qmsd_api_fd_set_cloexec(int fd)
{
	fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
}

static void qmsd_api_delete_process(struct qmsd_api_process *proc)
{
	list_del(&proc->list);
	ustream_free(&proc->log.stream);
	close(proc->log.fd.fd);
	free(proc);
}

static void qmsd_api_process_read_cb(struct ustream *s, int bytes)
{
	struct qmsd_api_process *proc;
	char *data;
	int len = 0;

	proc = container_of(s, struct qmsd_api_process, log.stream);

	do {
		char *newline;

		data = ustream_get_read_buf(s, &len);
		if (!len)
			break;

		newline = strchr(data, '\n');

		if (proc->log_overflow) {
			if (newline) {
				len = newline + 1 - data;
				proc->log_overflow = false;
			}
		} else if (newline) {
			*newline = 0;
			len = newline + 1 - data;
			qmsd_api_call_cb(data);
		} else if (len == s->r.buffer_len) {
			proc->log_overflow = true;
		} else
			break;

		ustream_consume(s, len);
	} while (1);
}

static void qmsd_api_process_cb(struct uloop_process *proc, int ret)
{
	struct qmsd_api_process *lp;
	lp = container_of(proc, struct qmsd_api_process, uloop);

	if(WIFEXITED(ret)) {
		printf("%s (%d): exit with %d\n",
			lp->process_name, lp->uloop.pid, WEXITSTATUS(ret));
	} else if (WIFSIGNALED(ret)) {
		printf("%s (%d): exit with signal %d\n",
			lp->process_name, lp->uloop.pid, WTERMSIG(ret));
	} else if (WIFSTOPPED(ret)) {
		printf("%s (%d): stop with signal %d\n",
			lp->process_name, lp->uloop.pid, WSTOPSIG(ret));
	}

	while (ustream_poll(&lp->log.stream));
	qmsd_api_delete_process(lp);
}

static int __qmsd_api_call(const char *process_argv)
{
	int pfds[2];
	pid_t pid;
	struct qmsd_api_process *l_proc = NULL;

	if (pipe(pfds) < 0) {
		fprintf(stderr, "pipe failed for (%d)", errno);
		return -1;
	}

	/*
	 * free after process stop
	 */
	l_proc = malloc(sizeof(struct qmsd_api_process));

	memset(l_proc, 0, sizeof(struct qmsd_api_process));

	if (!l_proc) {
		fprintf(stderr, "malloc qmsd_api_process failed");
		goto error;
	}

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork qmsd_api_process failed for");
		goto error;
	}

	if (!pid) {
		/* child */
		int i;
		char app_path[PATH_MAX];

		/*
		 * disable pipe read
		 */
		close(pfds[0]);

		/*
		 * stdin/sdtout/stderr to pipe write
		 */
		for (i = 0; i <= 2; i++) {
			if (pfds[1] == i)
			continue;

			dup2(pfds[1], i);
		}

		if (pfds[1] > 2)
			close(pfds[1]);

		memset(app_path, 0, sizeof(app_path));
		snprintf(app_path, sizeof(app_path), "%s%s", QMSDAPI_PATH, QMSDAPI_PROCESS_NAME);
		execl(app_path, QMSDAPI_PROCESS_NAME, QMSDAPI_QMSD_ARGV, process_argv, (char *)NULL);
		fprintf(stderr, "failed execl %s", app_path);
		exit(127);
	}

	/*
	 * parent disable pipe write
	 */
	close(pfds[1]);
	strncpy(l_proc->process_name, QMSDAPI_PROCESS_NAME, sizeof(l_proc->process_name) - 1);
	l_proc->uloop.cb = qmsd_api_process_cb;
	l_proc->uloop.pid = pid;
	uloop_process_add(&l_proc->uloop);
	list_add_tail(&l_proc->list, &qmsd_api_process_list);

	qmsd_api_fd_set_cloexec(pfds[0]);
	l_proc->log.stream.string_data = true;
	l_proc->log.stream.notify_read = qmsd_api_process_read_cb;
	l_proc->log.stream.r.buffer_len = QMSDAPI_MAX_RECV_BUFFER;
	ustream_fd_init(&l_proc->log, pfds[0]);

	return 0;

error:
	if (l_proc) {
		free(l_proc);
	}

	close(pfds[0]);
	close(pfds[1]);
	return -1;
}

int qmsd_api_call(const char *api_path, const char *api_name, cJSON *param)
{
    cJSON *cobj;
    int ret;

    cobj = cJSON_CreateObject();
    if (cobj) {
        char *json_arg;

        cJSON_AddItemToObject(cobj, "ver", cJSON_CreateString("1.0"));
        cJSON_AddItemToObject(cobj, "module", cJSON_CreateString(api_path));
        cJSON_AddItemToObject(cobj, "api", cJSON_CreateString(api_name));
        if (param) {
            cJSON_AddItemToObject(cobj, "param", param);
        }

        json_arg = cJSON_PrintUnformatted(cobj);

        if (json_arg) {
            ret = __qmsd_api_call(json_arg);
            free(json_arg);
            cJSON_Delete(cobj);
            return ret;
        }
    } else {
        if (param)
            cJSON_Delete(param);
    }

    return -1;
}