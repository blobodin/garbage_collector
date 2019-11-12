# -m 10000

a = {"value": 1, "prev": None, "next": None}
b = {"value": True, "prev": a, "next": None}
a["next"] = b
c = {"value": "three", "prev": b, "next": None}
b["next"] = c
d = {"value": [4, 5], "prev": c, "next": None}
c["next"] = d
# output 1624 bytes in use; 33 refs in use
mem()
gc()
# output 1624 bytes in use; 33 refs in use
mem()
# output 1 True three [4, 5]
print(a["value"], a["next"]["value"], a["next"]["next"]["value"], a["next"]["next"]["next"]["value"])
# output 1 True three [4, 5]
print(d["prev"]["prev"]["prev"]["value"], d["prev"]["prev"]["value"], d["prev"]["value"], d["value"])

# Remove b from the list
del b
a["next"] = c
c["prev"] = a
# output 1288 bytes in use; 27 refs in use
mem()
gc()
# output 1288 bytes in use; 27 refs in use
mem()
# output 1 three [4, 5]
print(a["value"], a["next"]["value"], a["next"]["next"]["value"])
# output 1 three [4, 5]
print(d["prev"]["prev"]["value"], d["prev"]["value"], d["value"])

# Remove references to head and tail
del a
gc()
# output 1288 bytes in use; 27 refs in use
mem()
# output 1 three [4, 5]
print(c["prev"]["value"], c["value"], c["next"]["value"])
del d
gc()
# output 1288 bytes in use; 27 refs in use
mem()
# output 1 three [4, 5]
print(c["prev"]["value"], c["value"], c["next"]["value"])

# Remove remaining reference to list
del c
# output 1288 bytes in use; 27 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()


# Another example; we cut the list into two self-referencing parts
a = {"value": 1, "prev": None, "next": None}
b = {"value": 2, "prev": a, "next": None}
a["next"] = b
c = {"value": 3, "prev": b, "next": None}
b["next"] = c
d = {"value": 4, "prev": c, "next": None}
c["next"] = d
e = {"value": 5, "prev": d, "next": None}
d["next"] = e
del a
del b
del d
del e
# output 1912 bytes in use; 38 refs in use
mem()
gc()
# output 1912 bytes in use; 38 refs in use
mem()
# output 1 2 3 4 5
print(c["prev"]["prev"]["value"], c["prev"]["value"], c["value"], c["next"]["value"], c["next"]["next"]["value"])
c["prev"]["next"] = None
c["next"]["prev"] = None
# output 1 2 3 4 5
print(c["prev"]["prev"]["value"], c["prev"]["value"], c["value"], c["next"]["value"], c["next"]["next"]["value"])
# output 1912 bytes in use; 38 refs in use
mem()
gc()
# output 1912 bytes in use; 38 refs in use
mem()
del c
# output 1544 bytes in use; 31 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
