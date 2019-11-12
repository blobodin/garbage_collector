# -m 416

# output 72 bytes in use; 3 refs in use
mem()
# output FizzBuzz
# output 1
# output 2
# output Fizz
# output 4
# output Buzz
# output Fizz
# output 7
# output 8
# output Fizz
# output Buzz
# output 11
# output Fizz
# output 13
# output 14
# output FizzBuzz
# output 16
# output 17
# output Fizz
# output 19
# output Buzz
# output Fizz
# output 22
# output 23
# output Fizz
# output Buzz
# output 26
# output Fizz
# output 28
# output 29
# output FizzBuzz
# output 31
# output 32
# output Fizz
# output 34
# output Buzz
# output Fizz
# output 37
# output 38
# output Fizz
# output Buzz
# output 41
# output Fizz
# output 43
# output 44
# output FizzBuzz
# output 46
# output 47
# output Fizz
# output 49
# output Buzz
# output Fizz
# output 52
# output 53
# output Fizz
# output Buzz
# output 56
# output Fizz
# output 58
# output 59
# output FizzBuzz
# output 61
# output 62
# output Fizz
# output 64
# output Buzz
# output Fizz
# output 67
# output 68
# output Fizz
# output Buzz
# output 71
# output Fizz
# output 73
# output 74
# output FizzBuzz
# output 76
# output 77
# output Fizz
# output 79
# output Buzz
# output Fizz
# output 82
# output 83
# output Fizz
# output Buzz
# output 86
# output Fizz
# output 88
# output 89
# output FizzBuzz
# output 91
# output 92
# output Fizz
# output 94
# output Buzz
# output Fizz
# output 97
# output 98
# output Fizz
i = 0
while i < 100:
    if i % 3 == 0 and i % 5 == 0:
        print("FizzBuzz")
    elif i % 3 == 0:
        print("Fizz")
    elif i % 5 == 0:
        print("Buzz")
    else:
        print(i)
    i = i + 1
del i
# output 72 bytes in use; 3 refs in use
mem()
