# -m 10000

# Testing getting values out of a dict
d = {10: 20, 30: "forty"}
# output {10: 20, 30: "forty"}
print(d)
# output 440 bytes in use; 10 refs in use
mem()
# output 20
print(d[10])
# output 440 bytes in use; 10 refs in use
mem()
x = d[30]
# output 440 bytes in use; 10 refs in use
mem()
del d
# output forty
print(x)
# output 104 bytes in use; 4 refs in use
mem()
del x
# output 72 bytes in use; 3 refs in use
mem()

# Test setting values in a dict
d = {}
# output {}
print(d)
# output 312 bytes in use; 6 refs in use
mem()
d[5] = [1, 2, 3]
# output {5: [1, 2, 3]}
print(d)
# output 528 bytes in use; 12 refs in use
mem()
k = 21
d[k] = 4
# output {5: [1, 2, 3], 21: 4}
print(d)
# output 592 bytes in use; 14 refs in use
mem()
del k
# output 592 bytes in use; 14 refs in use
mem()
# Overwrite existing value
d[5] = d[21]
# output {5: 4, 21: 4}
print(d)
# output 408 bytes in use; 9 refs in use
mem()
x = d[5]
# output 408 bytes in use; 9 refs in use
mem()
del d
# output 4
print(x)
# output 104 bytes in use; 4 refs in use
mem()
del x
# output 72 bytes in use; 3 refs in use
mem()

# Test deleting values from a dict
d = {"abc": 1, "def": 2, "ghi": 3}
# output {"def": 2, "ghi": 3, "abc": 1}
print(d)
# output 504 bytes in use; 12 refs in use
mem()
del d["abc"]
# output {"def": 2, "ghi": 3}
print(d)
# output 440 bytes in use; 10 refs in use
mem()
x = d["def"]
y = d["ghi"]
# output 440 bytes in use; 10 refs in use
mem()
del d["ghi"]
# output 408 bytes in use; 9 refs in use
mem()
del d
# output 136 bytes in use; 5 refs in use
mem()
# output 2
print(x)
# output 3
print(y)
del x
del y
# output 72 bytes in use; 3 refs in use
mem()

# Test tombstones
d = {1: [None], 17: [True], 34: [False]}
# output {1: [None], 17: [True], 34: [False]}
print(d)
# output 656 bytes in use; 15 refs in use
mem()
del d[1]
# output {17: [True], 34: [False]}
print(d)
# output [True]
print(d[17])
# output [False]
print(d[34])
# output 536 bytes in use; 12 refs in use
mem()
d[17] = "changed"
# output {17: "changed", 34: [False]}
print(d)
# output changed
print(d[17])
# output [False]
print(d[34])
# output 488 bytes in use; 11 refs in use
mem()
d[1] = "added"
# output {1: "added", 17: "changed", 34: [False]}
print(d)
# output added
print(d[1])
# output changed
print(d[17])
# output [False]
print(d[34])
# output 552 bytes in use; 13 refs in use
mem()
x = d[17]
del d[17]
del d[1]
# output {34: [False]}
print(d)
# output [False]
print(d[34])
# output changed
print(x)
# output 456 bytes in use; 10 refs in use
mem()
d[17] = 2
# output {17: 2, 34: [False]}
print(d)
# output 2
print(d[17])
# output [False]
print(d[34])
# output 520 bytes in use; 12 refs in use
mem()
del d[34]
# output {17: 2}
print(d)
# output 2
print(d[17])
# output changed
print(x)
# output 408 bytes in use; 9 refs in use
mem()
x = 33
d[x] = x
del x
# output {17: 2, 33: 33}
print(d)
# output 2
print(d[17])
# output 33
print(d[33])
# output 408 bytes in use; 9 refs in use
mem()
del d
# output 72 bytes in use; 3 refs in use
mem()
