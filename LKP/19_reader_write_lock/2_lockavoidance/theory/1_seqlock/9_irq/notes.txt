Seqlocks have other variants to use in interrupts.

write_seqlock_irq(seqlock_t *sl);

write_sequnlock_irq(seqlock_t *sl);

write_seqlock_irqsave(seqlock_t *sl);

write_sequnlock_irqrestore(seqlock_t *sl, unsigned long flags);
