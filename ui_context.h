#ifndef _UI_CONTEXT_H_
#define _UI_CONTEXT_H_

#define MAX_LAYER 5

#include "ui_widget.h"

typedef struct CUI_Context CUI_Context;
struct CUI_Context
{
    CUI_RootDir* root;              //控制的根目录(主菜单)
    CUI_List* list_p[MAX_LAYER];    //每层列表指针
    u8 layer;                       //当前层数
    CUI_Page* page;                 //当前的页面 
};

CUI_Context* CUI_Context_Create(CUI_RootDir* root);

#endif //!_UI_CONTEXT_H_