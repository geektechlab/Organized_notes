long (*compat_ioctl) (struct file *filp, unsigned int cmd, 
                          unsigned long arg);

Called when a 32-bit process calls ioctl() on a 64-bit system

It should then do whatever is required to convert the argument to native data types and carry out the request.
