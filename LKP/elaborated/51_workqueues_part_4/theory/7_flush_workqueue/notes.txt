flush_workqueue
---------------

void flush_workqueue(struct workqueue_struct *wq);

This function sleeps until all work items which were queued on entry have been finished.

typically used in driver shutdown handlers
