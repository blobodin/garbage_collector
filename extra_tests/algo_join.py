# -m 1744

# output 72 bytes in use; 3 refs in use
mem()
list = ["This", "is", "a", "sentence", "in", "a", "list."]
joined = list[0]
idx = 1
while idx < len(list):
    joined = joined + " " + list[idx]
    idx = idx + 1
# output This is a sentence in a list.
print(joined)
del list
del joined
del idx
# output 72 bytes in use; 3 refs in use
mem()
