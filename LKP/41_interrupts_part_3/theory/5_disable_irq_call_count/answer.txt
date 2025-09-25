What happens if i call disable_irq twice and enable_irq once?
=============================================================

Calls to these functions nest

For each call to disable_irq() or disable_irq_nosync() on a given interrupt line, a corresponding call to enable_irq() is required. 

Only on the last call to enable_irq() is the interrupt line actually enabled.

For example, if disable_irq() is called twice, the interrupt line is not actually reenabled until the second call to enable_irq().
