#include "ui.h"

lv_obj_t *createButton(lv_obj_t *parent, const char *text) {

  lv_obj_t *button = lv_button_create(parent);
  if (text != NULL) {
    lv_obj_t *buttonLabel = lv_label_create(button);
    lv_label_set_text(buttonLabel, text);
  }
  return button;
}