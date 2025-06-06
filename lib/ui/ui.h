#ifndef UI_H 
#define UI_H
//Function declaration 

#if __has_include("lvgl.h")
#include "lvgl.h"
#include "components/button.h"

#endif
#if __has_include("button.h")
#endif


#ifdef __cplusplus
extern "C" {
void ui_init(void);
extern void searchWifiDevices(void);
}
#endif

#endif //UI_H