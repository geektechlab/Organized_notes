spinlock vs mutexes
=======================

	Requirement				Recommended Lock
	=========================================================

	Low overhead locking			Spinlock

	Short lock hold time			Spinlock

	Long lock hold time			Mutex

	Need to lock from interrupt context	Spinlock

	Need to sleep while holding lock	Mutex
