# -m 10000

a = 5
# output 5
print(a)
# output 104 bytes in use; 4 refs in use
mem()
a = [1]
# output [1]
print(a)
# output 184 bytes in use; 6 refs in use
mem()
a[0] = a
# output [[[[...]]]]
print(a)
# output 152 bytes in use; 5 refs in use
mem()

gc()
# output [[[[...]]]]
print(a)
del a
# output 152 bytes in use; 5 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
