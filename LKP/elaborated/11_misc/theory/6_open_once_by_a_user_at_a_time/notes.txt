Restricting Access to a Single User at a Time
===============================================

Allow a single user to open a device in multiple processes but allow only one user to have the device open at a time.

To achieve this, two items are needed:
	an open count
	uid of the owner of the device.

The open call grants access on first open but remembers the owner of the device.
