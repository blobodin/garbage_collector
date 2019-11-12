# -m 1856

# output 72 bytes in use; 3 refs in use
mem()
list = [76, 34, 63, 21, 74, 37, 51, 25, 71, 19, 7, 4, 98, 1, 11, 54, 62, 87, 99]
# output [76, 34, 63, 21, 74, 37, 51, 25, 71, 19, 7, 4, 98, 1, 11, 54, 62, 87, 99]
print(list)
idx = 1
while idx < len(list):
    list[idx] = list[idx] + list[idx - 1]
    idx = idx + 1
# output [76, 110, 173, 194, 268, 305, 356, 381, 452, 471, 478, 482, 580, 581, 592, 646, 708, 795, 894]
print(list)
# output 864 bytes in use; 25 refs in use
mem()
del list
del idx
# output 72 bytes in use; 3 refs in use
mem()
