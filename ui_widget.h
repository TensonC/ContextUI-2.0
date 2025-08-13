#ifndef _UI_WIDGET_H_
#define _UI_WIDGET_H_

typedef unsigned char u8;

#define MENU_PIC_LARGE 72

typedef struct CUI_List CUI_List;
typedef struct CUI_Tab CUI_Tab;
typedef struct CUI_Widget CUI_Widget;
typedef struct CUI_Menu CUI_Menu;
typedef struct CUI_Page CUI_Page;
typedef struct CUI_RootDir CUI_RootDir;

typedef void(*res_fn)();
typedef void(*list_res_fn)(CUI_List* list);
typedef void(*tab_res_fn)(CUI_Tab* tab);
typedef void(*widget_res_fn)(CUI_Widget* tab);
typedef void(*menu_res_fn)(CUI_Menu* menu);
typedef void(*mainmenu_res_fn)(CUI_RootDir* root);


//列表结构体
struct CUI_List {
    u8 current_tab;         //当前的tab项
    u8 tab_count;           //tab个数
    CUI_Tab** tabs;         //tab指针

    list_res_fn up;     //上移函数
    list_res_fn down;   //下移函数
    list_res_fn enter;  //确定函数
    list_res_fn exit;   //退出函数
};

//列表项结构体
struct CUI_Tab {
    const char* title;              //名称
    enum LinkType union_flag;            //联合体标志
    union {
        CUI_List* next_list;        //下级列表
        CUI_Widget* link_widget;    //关联组件
        CUI_Page* link_page;        //关联页面
    } link ;
    void(*res)(CUI_Tab* tab);                 //响应函数
};

//组件结构体
struct CUI_Widget {
    enum WidgetType widget_type;        //组件种类，用来确定构造函数
    union {
        u8 is_check;                   //checkbox组件是否被选中
        u8 value;                      //number或percent bar类组件的数值
        const char* str;                //string组件的内容
    } content;
    struct {                            //位置信息，用来构建组件
        u8 top;
        u8 right;
    } position;
    widget_res_fn widget_res;            //组件响应函数
    res_fn res;
};

//页面结构体
struct CUI_Page
{
    res_fn page_build;          //页面构建函数

    res_fn up;                  //上移函数
    res_fn down;                //下移函数
    res_fn enter;               //确定函数
    res_fn exit;                //退出函数
};


//菜单项结构体
struct CUI_Menu
{
    const char* title;              //菜单项名称
    u8* pic;         //菜单图片
    enum LinkType unionflag;        //联合体标志
    union {
        CUI_List* link_list;
        CUI_Page* link_page;
    } link;
    menu_res_fn res;                //菜单的响应函数        
};

//主菜单结构体
struct CUI_RootDir
{
    u8 menu_count;            //菜单数量
    u8 current_menu;        //当前菜单
    CUI_Menu** menus;       //菜单指针

    mainmenu_res_fn up;         //左移函数
    mainmenu_res_fn down;       //右移函数
    mainmenu_res_fn enter;      //确定函数
    mainmenu_res_fn exit;       //退出函数
};


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

CUI_List* CUI_List_Create(CUI_Tab* tabs[],u8 tab_count);
CUI_Tab* CUI_Tab_Create(const char* title,enum LinkType link_type,void* link);
CUI_Widget* CUI_Widget_Create(enum WidgetType widget_type,void* content,u8 top,u8 right,res_fn res);
CUI_Page* CUI_Page_Create(res_fn page_build,res_fn up,res_fn down,res_fn enter,res_fn exit);
CUI_Menu* CUI_Menu_Create(const char* title,u8 pic[MENU_PIC_LARGE],enum LinkType link_type,void* link);
CUI_RootDir* CUI_RootDir_Create(CUI_Menu* menus[],u8 menu_count);


#endif //!_UI_WIDGET_H_