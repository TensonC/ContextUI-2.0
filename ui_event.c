#include "ui_event.h"




/**
 * @brief 事件队列初始化函数
 * @param event_queue 事件队列结构体指针
 */
void Event_Init(EventQueue* event_queue)
{
    event_queue->head = 0;
    event_queue->tail = 0;
}

/**
 * @brief 判断该队列是否已满
 * @param event_queue 事件队列结构体指针
 * @returns 1 已满 0 未满
 */
unsigned char isQueueFull(EventQueue* event_queue)
{
    if((event_queue->tail + 1) % MAX_QUEUE_LEN == event_queue->head)
        return 1;
    else 
        return 0;
}

/**
 * @brief 判断该队列是否为空
 * @param event_queue 事件队列结构体指针
 * @returns 1 空 0 不是空
 */
unsigned char isQueueEmpty(EventQueue* event_queue)
{
    if(event_queue->tail == event_queue->head)
        return 1;
    else 
        return 0;
}

/**
 * @brief 放入一个事件
 * @param event_queue 事件队列结构体指针
 * @param event_type 事件类型
 * @returns 0 队列已满 1 插入成功 
 */
static unsigned char Event_Write(EventQueue* event_queue,EventType event_type)
{
    if(isQueueFull(event_queue))
        return 0;
    event_queue->tail = (event_queue->tail + 1) % MAX_QUEUE_LEN;
    event_queue->queue[event_queue->tail] = event_type;
    return 1;
}

/**
 * @brief 读取并删除该事件
 * @param event_queue 事件队列结构体指针
 * @returns 事件类型
 */
EventType Event_Read(EventQueue* event_queue)
{
    if(isQueueEmpty(event_queue))
        return KEY_NONE;
    event_queue->head = (event_queue->head + 1) % MAX_QUEUE_LEN;
    return event_queue->queue[event_queue->head];
}

/**
 * @brief 输入Enter键
 * @param event_queue 要插入的队列
 */
void Input_Enter(EventQueue* event_queue)
{
    Event_Write(event_queue,KEY_ENTER);
}

/**
 * @brief 输入Exit键
 * @param event_queue 要插入的队列
 */
void Input_Exit(EventQueue* event_queue)
{
    Event_Write(event_queue,KEY_EXIT);
}

/**
 * @brief 输入Left键
 * @param event_queue 要插入的队列
 */
void Input_Left(EventQueue* event_queue)
{
    Event_Write(event_queue,KEY_LEFT);
}

/**
 * @brief 输入Right键
 * @param event_queue 要插入的队列
 */
void Input_Right(EventQueue* event_queue)
{
    Event_Write(event_queue,KEY_RIGHT);
}