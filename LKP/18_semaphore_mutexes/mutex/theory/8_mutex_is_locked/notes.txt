Test if the mutex is taken:
   int mutex_is_locked(struct mutex *lock);

Return:

	1 -> Locked
	0 -> Unlocked
