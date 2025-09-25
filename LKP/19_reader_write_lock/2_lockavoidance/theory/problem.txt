Problem
----------------

Readers-writer lock is a special lock mechanism which allows concurrent access for read-only operations

An exclusive lock is needed for writing or modifying data.

A writer process can't acquire a lock as long as at least one reader process which acquired a lock holds it.

This may lead to a problem called writer starvation, where writer process may sometimes need to wait long time for the lock
