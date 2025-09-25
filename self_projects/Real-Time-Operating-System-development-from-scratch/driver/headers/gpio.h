#include <stdint.h>
#include <stdbool.h>
#include "common_driv.h"

#ifndef GPIO_H
#define GPIO_H

void init_GPIO(GPIO_TypeDef *, uint32_t);
void toggle_GPIO(GPIO_TypeDef *, uint32_t);
void invert_GPIO(GPIO_TypeDef *, uint32_t);
void write_GPIO(GPIO_TypeDef *, uint32_t, bool);

#endif /* GPIO_H */
