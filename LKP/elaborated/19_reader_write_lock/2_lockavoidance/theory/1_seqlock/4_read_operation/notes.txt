Read Operation
--------------------

Read Operation:
-----------------

No locking is required when you are using trying to read.
Each reader must read the sequence number twice
	before reading the data
	after reading the data

And verify whether they both are same or not.

When there was no writer during the read operation?
-----------------------------------------------------

	value of the sequence counter will be same

When there was writer during the read operation?
----------------------------------------------------

	value of the sequence counter will not be same
	The important point here is that while the writer is operating, the sequence number would be odd.

unsigned int seq;

do {
	seq = read_seqbegin(&the_lock);
	/* Make a copy of the data of interest */
} while read_seqretry(&the_lock, seq);


read_seqbegin   -->     Returns the current sequence number.

read_seqretry	-->	Returns 1 if the value of seq local variable is odd.
