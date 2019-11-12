# Taken from https://projecteuler.net/problem=40.
# This test takes about 40 seconds.

# -m 1616

length = 0
next = 1
digits = [0, 0, 0, 0, 0, 0]
while length < 1000000:
    rest = next
    next_len = 0
    while rest:
        digit = rest % 10
        rest = (rest - digit) / 10
        digits[next_len] = digit
        next_len = next_len + 1
    new_length = length + next_len
    if length < 1 and new_length >= 1:
        d1 = digits[new_length - 1]
    elif length < 10 and new_length >= 10:
        d10 = digits[new_length - 10]
    elif length < 100 and new_length >= 100:
        d100 = digits[new_length - 100]
    elif length < 1000 and new_length >= 1000:
        d1000 = digits[new_length - 1000]
    elif length < 10000 and new_length >= 10000:
        d10000 = digits[new_length - 10000]
    elif length < 100000 and new_length >= 100000:
        d100000 = digits[new_length - 100000]
    elif new_length >= 1000000:
        d1000000 = digits[new_length - 1000000]
    length = new_length
    next = next + 1

del length
del next
del digits
del rest
del next_len
del digit
del new_length
# output 296 bytes in use; 10 refs in use
mem()
# output 210
print(d1 * d10 * d100 * d1000 * d10000 * d100000 * d1000000)
del d1
del d10
del d100
del d1000
del d10000
del d100000
del d1000000
# output 72 bytes in use; 3 refs in use
mem()
