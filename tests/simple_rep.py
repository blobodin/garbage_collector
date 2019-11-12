# -m 1616

a = [0]
a[0] = a
b = [0]
c = [0]
b[0] = c
c[0] = b
d = {0: b}
e = d
f = [[0]]
f[0][0] = f
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
gc()
# output 744 bytes in use; 17 refs in use
mem()
