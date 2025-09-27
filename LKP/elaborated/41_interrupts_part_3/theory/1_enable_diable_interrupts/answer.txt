Why do we need to disable interrupts?
=====================================

Disabling interrupts, you can guarantee that an interrupt handler will not preempt your current code.

Disabling interrupts also disables kernel preemption

Note: Disabling kernel preemption doesnot provide protection from concurrent access from another processor.
Use locks to prevent another processor from accessing shared data simultaneously.
