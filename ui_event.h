#ifndef _UI_EVENT_H_
#define _UI_EVENT_H_

//事件队列的最大长度
#define MAX_QUEUE_LEN 7

//按键类型
enum EventType {
    KEY_NONE,
    KEY_ENTER,
    KEY_EXIT,
    KEY_LEFT,
    KEY_RIGHT
};

typedef struct 
{
    //事件队列
    enum EventType queue[MAX_QUEUE_LEN];
    //队列头
    unsigned char head;
    //队列尾
    unsigned char tail;
    //当前事件
    enum EventType current_event;

} EventQueue;


EventQueue* CUI_Event_Create();
unsigned char isQueueFull(EventQueue* event_queue);
unsigned char isQueueEmpty(EventQueue* event_queue);
enum EventType CUI_Event_Read(EventQueue* event_queue);
void Input_Enter(EventQueue* event_queue);
void Input_Exit(EventQueue* event_queue);
void Input_Left(EventQueue* event_queue);
void Input_Right(EventQueue* event_queue);

#endif  //!_UI_EVENT_H_