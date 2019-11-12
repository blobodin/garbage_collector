# -m 1504

a = [0, 1, 2, 3]
b = {}
i = 0
while i < 10000:
    a[i % 4] = i
    b[i % 4] = i
    i = i + 1

# output [9996, 9997, 9998, 9999] {0: 9996, 1: 9997, 2: 9998, 3: 9999} 10000
print(a, b, i)
# output 688 bytes in use; 17 refs in use
mem()
