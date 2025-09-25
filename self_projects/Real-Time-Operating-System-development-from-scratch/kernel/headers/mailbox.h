#include <thread.h>
#include <semaphore.h>

#ifndef MAILBOX_H
#define MAILBOX_H

/* APIs to initialize, send and receive data using mailbox */
void p_os_mailbox_init(void);
void p_os_mailbox_send(uint32_t data);
uint32_t p_os_mailbox_recv();

#endif /* MAILBOX_H */
