#ifndef _UI_WIDGET_H_
#define _UI_WIDGET_H_

typedef unsigned char u8;

typedef struct CUI_List CUI_List;
typedef struct CUI_Tab CUI_Tab;
typedef struct CUI_Widget CUI_Widget;
typedef struct CUI_Menu CUI_Menu;
typedef struct CUI_Page CUI_Page;
typedef struct CUI_MainMenu CUI_MainMenu;

typedef void(*res_fn)();

//组件类型的枚举，用于确定构建函数
enum WidgetType {
    PERCENTBAR,
    CHECKBOX,
    STRING,
    NUMBER,
};

//联合体内容的枚举，用于确定构建函数和响应函数
enum LinkType {
    WIDGET,
    LIST,
    PAGE
};


#endif //!_UI_WIDGET_H_