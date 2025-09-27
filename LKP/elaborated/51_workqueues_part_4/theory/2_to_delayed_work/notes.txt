static inline struct delayed_work *to_delayed_work(struct work_struct *work)
{
        return container_of(work, struct delayed_work, work);
}

Header File: <linux/workqueue.h>
