# -m 1056

# output 72 bytes in use; 3 refs in use
mem()
list = ["aetn", "nyej", "boeny", "now4y5", "xgfhoi", "iegnb", "jmigfh"]
# output ["aetn", "nyej", "boeny", "now4y5", "xgfhoi", "iegnb", "jmigfh"]
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
# output ["aetn", "boeny", "iegnb", "jmigfh", "now4y5", "nyej", "xgfhoi"]
print(list)
del list
del switch
del idx
del temp
# output 72 bytes in use; 3 refs in use
mem()
