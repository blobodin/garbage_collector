# -m 592

# output 72 bytes in use; 3 refs in use
mem()
# output 1
print(1)
# output 2
print(2)
# output 3
print(1 + 2)
# output 15
print(3 * 5)
# output 36
print(1 + 2 + 3 + 4 + 5 + 6 + 7 + 8)
# output 72 bytes in use; 3 refs in use
mem()
a = 4
b = 5
c = 6
d = b * c
e = a + b + c
f = 4444
g = f * e
# output 4 5 6 30 15 4444 66660
print(a, b, c, d, e, f, g)
# output 296 bytes in use; 10 refs in use
mem()
del a
del b
del c
del d
del e
del f
del g
# output 72 bytes in use; 3 refs in use
mem()
