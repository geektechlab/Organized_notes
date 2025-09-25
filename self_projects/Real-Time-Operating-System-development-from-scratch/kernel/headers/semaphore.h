#include <thread.h>

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

/* APIs to initialize, set and wait for semaphore */
void p_os_semaphore_init(int32_t *semaphore, int32_t value);
void p_os_semaphore_set(int32_t *semaphore);
void p_os_semaphore_wait(int32_t *semaphore);

#endif /* SEMAPHORE_H */
