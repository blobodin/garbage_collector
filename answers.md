# Q0
The allocator is implemented using an implicit list, meaning that to traverse
forward or backward over any values already allocated within the memory pool
it will need to know the sizes of each value. Using the sizes of the values in
memory, it can reach the address of the next or previous value accordingly.

# Q1
TOMBSTONE_REF is used to denote key slots that have been deleted. Dictionary
keys are stored in memory contiguously. If we could not denote it at such and
skip it while searching, then we would have to reorder the entries once
deleting the key.

# Q2
Free_value_t denotes a block of memory that is not currently allocated.
Since it is not allocated, there will be no references assigned to it.
The size is still necessary as it is used to determine whether or not the
block is compatible with a memory request.
