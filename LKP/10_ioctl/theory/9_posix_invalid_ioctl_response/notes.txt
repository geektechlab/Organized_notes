Return Value
==================

What should be the return value when invalid command is passed?

As per the POSIX Standard, if an inappropriate ioctl command has  been issued , then -ENOTTY should be returned.

ENOTTY	-->	Inappropriate IOCTL For the device
