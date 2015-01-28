#include "includes.h"


queue * define_new_queue(queue *pNewQueue, U16 queue_size)
{
    if(pNewQueue == NULL)
        return NULL;

    pNewQueue->first = 0;
    //pNewQueue->last = queue_size-1;    /* alan check - 貌似这里应该是: END_TX_QUEUE_SIZE  */
    pNewQueue->last = 0;
    pNewQueue->count = 0;
    pNewQueue->maxcount = queue_size;        // 50

    return pNewQueue;
}


U16 enqueue(queue *q, HANDLE x)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif
    if(q)
    {
        if (q->count >= q->maxcount)
        {
            //Alert(ALERT_FULL_QUEUE, ALERT_NO_ACTION, __FILE__, __LINE__);
            return FALSE;
        }
        else
        {
            OS_ENTER_CRITICAL();
            q->q[ q->last ] = x;
            q->count = q->count + 1;
            q->last = (q->last+1) % (q->maxcount);
            OS_EXIT_CRITICAL();
        }

    }
    return TRUE;
}

HANDLE dequeue(queue *q)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif
    HANDLE x = NULL;
    if(q)
    {

        if (q->count <= 0)
        {
            //printf("Warning: empty queue dequeue.\n");
            //alert();
            //return NULL;
        }
        else
        {

            OS_ENTER_CRITICAL();
            x = q->q[ q->first ];
            q->first = (q->first+1) % (q->maxcount);
            q->count = q->count - 1;
            OS_EXIT_CRITICAL();

        }
    }
    return(x);
}

int is_queue_empty(queue *q)
{
    if (q->count <= 0)
        return (TRUE);
    else
        return (FALSE);
}

USHORT get_queue_cnt(queue *q)
{
    if(q)
    {
        if ( (q->count > q->maxcount) )
        {
            //Alert(ALERT_FULL_QUEUE, ALERT_NO_ACTION, __FILE__, __LINE__);
        }
        else
        {
            return (q->count);
        }
    }

    return 0xFFFF;
}

