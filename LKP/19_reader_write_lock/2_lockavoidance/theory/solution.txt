Much effort has been put in developing synchronization primitives that avoid locks.

Lock free and wait free synchronization plays a major role in RTOS, where time guarantees must be given.

Two new synchronization mechanisms added in 2.6 Kernel to totally remove locking on the reader side:

	1. Sequence Lock

	2. Read Copy Update (RCU)
