#include <stdint.h>
#include "common.h"

#ifndef COMMON_DRIV_H
#define COMMON_DRIV_H

void _close(void);
void _lseek(void); 
void _read(void);
void _write(void);

void bsp_init(void);
#endif /* COMMON_DRIV_H */
