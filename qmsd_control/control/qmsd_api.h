#ifndef __QMSD_API_H
#define __QMSD_API_H

#include <libubox/list.h>
#include <libubox/uloop.h>
#include <libubox/ustream.h>

#include "cJSON.h"

typedef void (*qmsd_api_cb)(char *msg);

struct qmsd_api_process {
	struct list_head list;
	struct uloop_process uloop;

	struct ustream_fd log;
	bool log_overflow;
	char process_name[64];
};

void qmsd_api_set_cb(qmsd_api_cb cb);
int qmsd_api_call_cb(char *msg);

int qmsd_api_call(const char *api_path, const char *api_name, cJSON *param);

#endif
