# -m 432

# Use and free a 40-byte slot
s = "abcdefgh"
t = 10
# output abcdefgh 10
print(s, t)
# output 144 bytes in use; 5 refs in use
mem()
del s
# output 104 bytes in use; 4 refs in use
mem()
# Takes up the full 40-byte free slot
r = 99999
# output 99999 10
print(r, t)
# output 144 bytes in use; 5 refs in use
mem()

# Reset the pool
del r
del t
gc()

# Try again, but compact the free memory with a gc()
s = "abcdefgh"
t = 10
# output abcdefgh 10
print(s, t)
# output 144 bytes in use; 5 refs in use
mem()
del s
# output 104 bytes in use; 4 refs in use
mem()
gc()
# output 104 bytes in use; 4 refs in use
mem()
# Takes up only 32 bytes of the free space
r = 99999
# output 99999 10
print(r, t)
# output 136 bytes in use; 5 refs in use
mem()

# Reset the pool
del r
del t
gc()

# A value is free but due to fragmentation, there isn't enough space for an allocation
x = 1
l = [x]
del x
del l[0]
# output 152 bytes in use; 5 refs in use
mem()
# After collecting garbage, the free space is coalesced at the end of the heap
gc()
# output 152 bytes in use; 5 refs in use
mem()
# output this is a pretty long string
print("this is a pretty long string")
