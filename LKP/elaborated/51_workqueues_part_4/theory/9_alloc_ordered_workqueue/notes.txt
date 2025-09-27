alloc_ordered_workqueue
------------------------

#define alloc_ordered_workqueue(fmt, flags, args...)

Allocate an ordered workqueue

An ordered workqueue executes at most one work item at any given time in the queued order

They are implemented as unbound workqueues with max_active of one
