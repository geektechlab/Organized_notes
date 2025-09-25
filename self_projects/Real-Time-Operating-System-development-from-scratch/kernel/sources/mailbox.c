#include "mailbox.h"

/* Variables to support mailbox functionality */
static uint8_t mailbox_has_data;
static uint32_t mailbox_data;
static int32_t mailbox_semaphore;

/* Initialize a mailbox */
void p_os_mailbox_init(void)
{
    mailbox_has_data = 0;
    mailbox_data = 0 ;
    p_os_semaphore_init(&mailbox_semaphore, 0);
}

/* Send a data using mailbox */
void p_os_mailbox_send(uint32_t data)
{
    /* Atomic operation is required for sending data using mailbox */
    __disable_irq();

	if(mailbox_has_data)
	{
		__enable_irq();
		return;
	}

	mailbox_data = data;
	mailbox_has_data = 1;

	__enable_irq();

    p_os_semaphore_set(&mailbox_semaphore);
}

/* Receive a data from mailbox */
uint32_t p_os_mailbox_recv()
{
	p_os_semaphore_wait(&mailbox_semaphore);

	uint32_t data;

    /* Atomic operation required for semaphore read */
    __disable_irq();

	data = mailbox_data;
	mailbox_has_data = 0;

    __enable_irq();

    return data;
}
