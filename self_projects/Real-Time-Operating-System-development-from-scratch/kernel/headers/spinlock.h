#include <thread.h>

#ifndef SPINLOCK_H
#define SPINLOCK_H

/* APIs to initialize, set and wait for spinlock */
void p_os_spinlock_init(int32_t *semaphore, int32_t value);
void p_os_spinlock_set(int32_t *semaphore);
void p_os_spinlock_wait(int32_t *semaphore);

#endif /* SPINLOCK_H */
