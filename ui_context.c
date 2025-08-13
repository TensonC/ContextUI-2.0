#include "ui_context.h"
#include "stdlib.h"



/**
 * @brief Context的创建函数
 * @param root 根目录指针
 */
CUI_Context* CUI_Context_Create(CUI_RootDir* root) {
    CUI_Context* ctx = (CUI_Context*)malloc(sizeof(CUI_Context));
    if(!ctx)
        return;
    ctx->root = root;
    ctx->layer = 0;
    ctx->page = NULL;
    for(u8 i=0;i<MAX_LAYER;i++) {
        ctx->list_p[i] = NULL;
    }

    return ctx;
}
