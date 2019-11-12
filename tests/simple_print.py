# -m 400

# output 72 bytes in use; 3 refs in use
mem()
a = "Hello"
# output Hello world!
print(a, "world" + "!")
# output 104 bytes in use; 4 refs in use
mem()
del a
# output 72 bytes in use; 3 refs in use
mem()
