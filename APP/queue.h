#ifndef __QUEUE_H__
#define __QUEUE_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define S8    signed char
#define I8    signed char
#define U8    unsigned char     /* unsigned 8  bits. */
#define I16   signed short      /*   signed 16 bits. */
#define U16   unsigned short    /* unsigned 16 bits. */
#define I32   signed long       /*   signed 32 bits. */
#define U32   unsigned long     /* unsigned 32 bits. */
#define I16P  I16               /*   signed 16 bits OR MORE ! */
#define U16P  U16               /* unsigned 16 bits OR MORE ! */

typedef unsigned long ULONG;
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned char BOOL;
typedef unsigned char BYTE;
typedef unsigned char *PBYTE;

typedef unsigned int BIT_FIELD;
typedef void *pvoid;
typedef void VOID;
typedef void *PVOID;

#ifndef NULL
#define NULL       ((void *)0)
#endif

#ifndef HANDLE
#define HANDLE          PVOID
#endif

#define QUEUE_SIZE       1000

typedef struct
{
    USHORT first;                      /* position of first element */
    USHORT last;                       /* position of last element */
    USHORT count;                      /* number of queue elements */
    USHORT maxcount;                   /* max number of queue elements */
    //ULONG inQueueTimer;               /* time length in queue */
    HANDLE q[1];                    /* body of queue */
} queue;

queue * define_new_queue(queue *pNewQueue, U16 queue_size);
U16 enqueue(queue *q, HANDLE x);
HANDLE dequeue(queue *q);
USHORT get_queue_cnt(queue *q);


#ifdef __cplusplus
}
#endif

#endif
