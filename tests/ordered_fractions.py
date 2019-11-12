# Taken from https://projecteuler.net/problem=71

# -m 656

max_n = 0
max_d = 1
d = 900000
while d <= 1000000:
    three_d = d * 3 - 1
    n = (three_d - three_d % 7) / 7
    if n * max_d > d * max_n:
        max_n = n
        max_d = d
    d = d + 1
# output 428570
print(max_n)
# output 232 bytes in use; 8 refs in use
mem()
del max_d
del d
del three_d
del n
# output 104 bytes in use; 4 refs in use
mem()
