#ifndef __QMSD_CONTROL_H
#define __QMSD_CONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

void qmsd_control_init(int *pipe_fd);
void qmsd_control_deinit(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__QMSD_CONTROL_H
