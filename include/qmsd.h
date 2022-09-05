#ifndef __QMSD_H
#define __QMSD_H

#include "cJSON.h"

#define QMSD_VERSION	"2.1"

typedef enum {
	QMSD_DISP_ORIGIN,
	QMSD_DISP_90,
	QMSD_DISP_180,
	QMSD_DISP_270,
} qmsd_disp_dir;

void qmsd_init(void);
void qmsd_deinit(void);

void qmsd_disp_set_dir(qmsd_disp_dir dir);
qmsd_disp_dir qmsd_disp_get_dir(void);

#endif
