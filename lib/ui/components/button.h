#ifndef BUTTON_H 
#define BUTTON_H
 //Function declaration 


#if __has_include("lvgl")
#include "lvgl.h"

#endif

// #ifdef __cplusplus
// extern "C"{
    lv_obj_t *createButton(lv_obj_t *parent, const char *text);
// }

// #endif
#endif //BUTTON_H