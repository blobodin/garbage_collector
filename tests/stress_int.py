# -m 464

a = 0
b = 1
i = 0
while i < 10000:
    b = a + b
    a = b - a
    i = i + 1

# output -2872092127636481573 -83367563645688771 10000
print(a, b, i)
# output 168 bytes in use; 6 refs in use
mem()
