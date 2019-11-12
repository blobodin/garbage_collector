# -m 10000

a = [0]
a[0] = a

b = [0]
c = [0]
b[0] = c
c[0] = b
d = {0: b}
e = d

f = [[0]]
g = [[0]]
f[0][0] = g
g[0][0] = f

# output 904 bytes in use; 21 refs in use
mem()
gc()
# output 904 bytes in use; 21 refs in use
mem()
del f
del g
# output 904 bytes in use; 21 refs in use
mem()
gc()
# output 584 bytes in use; 13 refs in use
mem()
del b
gc()
# output 584 bytes in use; 13 refs in use
mem()
del d
gc()
# output 584 bytes in use; 13 refs in use
mem()
del e
# output 312 bytes in use; 9 refs in use
mem()
gc()
# output 312 bytes in use; 9 refs in use
mem()
del c
# output 312 bytes in use; 9 refs in use
mem()
gc()
# output 152 bytes in use; 5 refs in use
mem()
del a
# output 152 bytes in use; 5 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
