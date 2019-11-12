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
b = [a, a, a]
# output [[1], [1], [1]]
print(b)
# output 272 bytes in use; 8 refs in use
mem()
