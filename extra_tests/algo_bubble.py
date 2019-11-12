# -m 1920

# output 72 bytes in use; 3 refs in use
mem()
# output [76, 34, 63, 21, 74, 37, 51, 25, 71, 19, 7, 4, 98, 1, 11, 54, 62, 87, 99]
list = [76, 34, 63, 21, 74, 37, 51, 25, 71, 19, 7, 4, 98, 1, 11, 54, 62, 87, 99]
print(list)
switch = True
while switch:
    switch = False
    idx = 0
    while idx < len(list) - 1:
        if list[idx] > list[idx + 1]:
            switch = True
            temp = list[idx]
            list[idx] = list[idx + 1]
            list[idx + 1] = temp
        idx = idx + 1
# output [1, 4, 7, 11, 19, 21, 25, 34, 37, 51, 54, 62, 63, 71, 74, 76, 87, 98, 99]
print(list)
del list
del switch
del idx
del temp
# output 72 bytes in use; 3 refs in use
mem()
