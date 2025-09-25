CAP_SYS_ADMIN
===============

The ability to perform all the system administration operations. Almost near to root.

Before performing a privileged operation, a device driver should check that the calling process has the appropriate capability or not. 

Capabilities checks are performed with the capable function.

Header File: <linux/sched.h>

int capable(int capability);
