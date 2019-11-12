# -m 20304

trial = 0
while trial < 100:
    len = 0
    str = ""
    while len < 250:
        str = str + "x"
        len = len + 1
    trial = trial + 1

# output xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 250 100
print(str, len, trial)
# output 416 bytes in use; 6 refs in use
mem()
