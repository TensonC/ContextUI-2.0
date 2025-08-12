#include "ui_widget.h"
#include "stdlib.h"

#define MENU_PIC_LARGE 72

typedef void(*list_res_fn)(CUI_List* list);
typedef void(*tab_res_fn)(CUI_Tab* tab);
typedef void(*widget_res_fn)(CUI_Widget* tab);
typedef void(*menu_res_fn)(CUI_Menu* menu);
typedef void(*mainmenu_res_fn)(CUI_MainMenu* main_menu);

/*
界面类型
    MainMenu
    List
    Page
拥有操作响应函数

界面项类型
    Tab
        Widget
    Menu
*/


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
struct CUI_MainMenu
{
    u8 menu_num;            //菜单数量
    u8 current_menu;        //当前菜单
    CUI_Menu** menus;       //菜单指针

    mainmenu_res_fn up;         //左移函数
    mainmenu_res_fn down;       //右移函数
    mainmenu_res_fn enter;      //确定函数
    mainmenu_res_fn exit;       //退出函数
};


/**
 * @brief 向下滚动列表
 * @param list 当前列表
 */
static void list_downward(CUI_List* list) {
    if (list->current_tab + 1 < list->tab_count)
    {
        list->current_tab++;
    }
}

/**
 * @brief 向上滚动列表
 * @param list 当前列表
 */
static void list_upward(CUI_List* list) {
    if (list->current_tab > 0)
    {
        list->current_tab--;
    }
}

/**
 * @brief 确定函数,list的调用函数调用所在Tab的对应调用函数,ctx调用的始终为list的函数
 * @param list 当前列表
 */
static void list_enter(CUI_List* list) {
    list->tabs[list->current_tab]->res(list->tabs[list->current_tab]);
}

/**
 * @brief 退出函数
 * @param list 当前列表
 */
static void list_exit(CUI_List* list)
{
    //TODO:在context中待实现
    CUI_Return_last_list();
}

/**
 * @brief Tab的列表调用函数
 * @param tab 该tab
 */
static void tab_next_list_res(CUI_Tab* tab)
{
    //TODO:在context中待实现
    CUI_Enter_next_list(tab->link.next_list);
}

/**
 * @brief Tab的组件调用函数
 * @param tab 该tab
 */
static void tab_link_widget_res(CUI_Tab* tab)
{
    //直接调用关联组件的函数
    tab->link.link_widget->widget_res(tab->link.link_widget);
}

/**
 * @brief Tab的页面调用函数
 * @param tab 该tab
 */
static void tab_link_page_res(CUI_Tab* tab)
{
    //TODO:直接页面开启函数，待完成
    CUI_Open_Page(tab->link.link_page->page_build);
}

/**
 * @brief 选择框组件的调用函数
 * @param widget 组件
 */
static void widget_checkbox_click(CUI_Widget* widget)
{
    widget->content.is_check = !widget->content.is_check;
    widget->res();
}

/**
 * @brief 进度条组件的调用函数
 * @param widget 组件
 */
static void widget_percentbar_click(CUI_Widget* widget)
{
    //TODO:进度条组件的调用，类似于打开一个弹窗的功能实现
    CUI_Open_PercentBar(widget);
    widget->res();
}

/**
 * @brief 数字组件的调用函数
 * @param widget 组件
 */
static void widget_number_click(CUI_Widget* widget)
{
    widget->res();
}

/**
 * @brief 字符串组件的调用函数
 * @param widget 组件
 */
static void widget_string_click(CUI_Widget* widget)
{
    widget->res();
}

/**
 * @brief menu的列表调用函数
 * @param menu 该menu
 */
static void menu_next_list_res(CUI_Menu* menu)
{
    //TODO:在context中待实现
    CUI_Enter_next_list(menu->link.link_list);
}

/**
 * @brief menu的页面调用函数
 * @param menu 该menu
 */
static void menu_link_page_res(CUI_Menu* menu)
{
    //TODO:直接页面开启函数，待完成
    CUI_Open_Page(menu->link.link_page->page_build);
}

/**
 * @brief 向右滚动主菜单
 * @param main_menu 主菜单
 */
static void menu_rightward(CUI_MainMenu* main_menu) {
    if ( main_menu->current_menu + 1 < main_menu->menu_num)
    {
        main_menu->current_menu++;
    }
}

/**
 * @brief 向左滚动列表
 * @param main_menu 主菜单
 */
static void menu_leftward(CUI_MainMenu* main_menu) {
    if (main_menu->current_menu > 0)
    {
        main_menu->current_menu--;
    }
}

/**
 * @brief 进入某菜单,即调用该菜单下的响应函数
 * @param main_menu 主菜单
 */
static void menu_enter(CUI_MainMenu* main_menu) {
    main_menu->menus[main_menu->current_menu]->res(main_menu->menus[main_menu->current_menu]);
} 

/**
 * @brief 进入某菜单,即调用该菜单下的响应函数
 * @param main_menu 主菜单
 */
static void menu_exit(CUI_MainMenu* main_menu) {

} 

/**
 * @brief 创建一个列表
 * @param list 列表指针
 * @param tabs 列表项数组
 * @param tab_count 列表项个数
 * @return 列表地址
 */
CUI_List* CUI_List_Create(CUI_Tab* tabs[],u8 tab_count)
{
    CUI_List* list = (CUI_List*)malloc(sizeof (CUI_List));
    if(!list)
        return NULL;
    list->current_tab = 0;
    //列表中的Tab项，注意数组越界
    list->tabs = tabs;
    list->tab_count = tab_count;
    //列表对应的响应函数
    list->up = list_upward;
    list->down = list_downward;
    list->enter = list_enter;
    list->exit = list_exit;

    return list;
}

/**
 * @brief 创建一个列表项
 * @param title 列表项标题
 * @param link_type 列表项关联类型
 * @param link 列表项关联指针
 * @return 列表项地址
 */
CUI_Tab* CUI_Tab_Create(const char* title,enum LinkType link_type,void* link)
{
    CUI_Tab* tab = (CUI_Tab*)malloc(sizeof(CUI_Tab));
    if(!tab)
        return NULL;
    tab->title = title;
    tab->union_flag = link_type;
    switch (link_type)
    {
    case LIST:
        tab->link.next_list = (CUI_List*)link;
        tab->res = tab_next_list_res;
        break;
    case WIDGET:
        tab->link.link_widget = (CUI_Widget*)link;
        tab->res = tab_link_widget_res;
        break;
    case PAGE:
        tab->link.link_page = (CUI_Page*)link;
        tab->res = tab_link_page_res;
        break;
    default:
        break;
    }

    return tab;
}

/**
 * @brief 创建一个组件
 * @param widget_type 组件类型
 * @param content 组件内容
 * @param top 组件与列表顶端高度
 * @param right 组件与列表右侧高度
 * @param res 组件响应函数
 * @return 组件地址
 */
CUI_Widget* CUI_Widget_Create(enum WidgetType widget_type,void* content,u8 top,u8 right,res_fn res)
{
    CUI_Widget* widget = (CUI_Widget*)malloc(sizeof(CUI_Widget));
    if(!widget)
        return NULL;
    widget->widget_type = widget_type;
    widget->position.right = right;
    widget->position.top = top;
    widget->res = res;
    switch (widget_type)
    {
    case CHECKBOX:
        widget->widget_res = widget_checkbox_click;
        widget->content.is_check = (u8*)content;
        break;
    case NUMBER:
        widget->widget_res = widget_number_click;
        widget->content.value = (u8*)content;
        break;
    case PERCENTBAR:
        widget->widget_res = widget_percentbar_click;
        widget->content.value = (u8*)content;
        break;
    case STRING:
        widget->widget_res = widget_string_click;
        widget->content.str = (const char*)content;
        break;
    default:
        break;
    }

    return widget;
}

/**
 * @brief 创建一个页面
 * @param page_bulid 页面构建函数
 * @param up 上移按键操作
 * @param down 下移按键操作
 * @param enter 确定按键操作
 * @param exit 退出按键操作
 * @return 页面地址
 */
CUI_Page* CUI_Page_Create(res_fn page_build,res_fn up,res_fn down,res_fn enter,res_fn exit)
{
    CUI_Page* page = (CUI_Page*)malloc(sizeof(CUI_Page));
    if(!page)
        return NULL;
    page->page_build = page_build;
    page->up = up;
    page->down = down;
    page->enter = enter;
    page->exit = exit;

    return page;
}


/**
 * @brief 创建一个菜单
 * @param title 菜单名称
 * @param pic 菜单图标,大小为72字节,即24 * (24 / 8),共占24 * 24个像素点
 * @param link_type 关联类型
 * @param link 关联指针
 */
CUI_Menu* CUI_Menu_Create(const char* title,u8* pic[MENU_PIC_LARGE],enum LinkType link_type,void* link) 
{
    CUI_Menu* menu = (CUI_Menu*)malloc(sizeof(CUI_Menu));
    if(!menu)
        return NULL;
    menu->title = title;
    menu->pic = *pic;
    menu->unionflag = link_type;
    switch (link_type)
    {
    case LIST:
        menu->link.link_list = (CUI_List*)link;
        menu->res = menu_next_list_res; 
        break;
    case PAGE:
        menu->link.link_page = (CUI_Page*)link;
        menu->res = menu_link_page_res; 
        break;
    default:
        break;
    }

    return menu;
}

/**
 * @brief 为主菜单添加菜单
 * @param menus 菜单列表
 * @param menu_count 菜单个数
 */
CUI_MainMenu* CUI_MainMenu_Create(CUI_Menu* menus[],u8 menu_count)
{
    CUI_MainMenu* main_menu = (CUI_MainMenu*)malloc(sizeof(CUI_MainMenu));
    if(!main_menu)
        return NULL;
    main_menu->current_menu = 0;
    main_menu->menu_num = menu_count;
    main_menu->menus = menus;

    main_menu->down = menu_rightward;
    main_menu->up = menu_leftward;
    main_menu->enter = menu_enter;
    main_menu->exit = menu_exit;

    return main_menu;
}