# Taken from https://projecteuler.net/problem=112.
# This test takes about 2 minutes, so it isn't run by default.

# -m 656

bouncy = 0
test = 100
while not (bouncy == test * 99):
    test = test + 1
    last_d = test % 10
    rest = (test - last_d) / 10
    inc = True
    dec = True
    while (inc or dec) and rest:
        d = rest % 10
        rest = (rest - d) / 10
        inc = inc and d <= last_d
        dec = dec and d >= last_d
        last_d = d
    if not (inc or dec):
        bouncy = bouncy + 100
# output 1587000
print(test)
# output 200 bytes in use; 7 refs in use
mem()
