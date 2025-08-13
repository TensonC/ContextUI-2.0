#include "ui_event.h"
#include "ui_context.h"

u8 pic[] = {};
void CUI_Init(CUI_Context* ctx)
{
    u8 is_check = 0;
    void check();
    CUI_Widget* checkbox = CUI_Widget_Create(CHECKBOX,&is_check,6,6,check);
    CUI_Widget* str = CUI_Widget_Create(STRING,"hello",6,6,check);
    CUI_Tab* tab1 = CUI_Tab_Create("tab1",WIDGET,checkbox);
    CUI_Tab* tab2 = CUI_Tab_Create("tab2",WIDGET,str);
    CUI_Tab* list1_tabs[] = {tab1,tab2};
    CUI_List* list1 = CUI_List_Create(list1_tabs,2);
    CUI_Menu* menu1 = CUI_Menu_Create("menu1",pic,LIST,list1);
    CUI_Menu* menus[] = {menu1};
    CUI_RootDir* root = CUI_RootDir_Create(menus,1);

    ctx = CUI_Context_Create(root);
    
}

int main()
{
    
    EventQueue* event;
    CUI_Context* ctx;
    CUI_Event_Create(event);
    CUI_Init(ctx);

    while(1)
    {
        CUI_Event_Read(event);
        CUI_EventDistribution(event->current_event);
        UI_Update(ctx);
    }

}