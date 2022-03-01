#ifndef __QMSD_MSGQUE_H__
#define __QMSD_MSGQUE_H__

#define __need_timeval 

#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TRUE
#define TRUE    (1 == 1)
#define FALSE   (0 == 1)
#endif

#define MAX_MSG_COUNT 128

#define M_RET_OK	0
#define M_RET_FAIL	-1

typedef void * t_msg;
typedef void * (*t_malloc_handler)(unsigned long size);
typedef void (*t_free_handler)(void *mem_p);

typedef struct t_msg_block
{
    struct t_msg_block *nextp;
    t_msg   msg;
    unsigned short msglen;
}t_msg_block;

typedef struct
{
    pthread_mutex_t qmutex;
    t_msg_block *firstp;
    t_msg_block *lastp;
    int msgcount;
    int msgmax;
}t_qcb;

int qmsd_msgque_init(int msgmax);
int qmsd_msgque_send(void *msg,unsigned int msglen);
int qmsd_msgque_recv(void **msg,unsigned int *msglen);
void qmsd_msgque_deinit(void);

int qmsd_main_msgque_init(int msgmax);
int qmsd_main_msgque_send(void *msg,unsigned int msglen);
int qmsd_main_msgque_recv(void **msg,unsigned int *msglen);
void qmsd_main_msgque_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
