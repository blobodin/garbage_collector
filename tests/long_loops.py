# -m 100000

# Create three separate cycles
z = {"next": "i am root"}
y = {"next": z}
x = {"next": y}
w = {"next": x}
v = {"next": w}
u = {"next": v}
t = {"next": u}
s = {"next": t}
r = {"next": s}
q = {"next": r}
p = {"next": q}
o = {"next": p}
n = {"next": o}
m = {"next": n}
l = {"next": m}
k = {"next": l}
j = {"next": k}
i = {"next": j}
h = {"next": i}
g = {"next": h}
f = {"next": g}
e = {"next": f}
d = {"next": e}
c = {"next": d}
b = {"next": c}
a = {"next": b}
z["next"] = n
m["next"] = b
a["next"] = a
# output {"next": {"next": {"next": {...: ...}}}}
print(a)
# output {"next": {"next": {"next": {...: ...}}}}
print(b)
# output {"next": {"next": {"next": {...: ...}}}}
print(m)
# output {"next": {"next": {"next": {...: ...}}}}
print(n)
# output {"next": {"next": {"next": {...: ...}}}}
print(z)
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del z
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del m
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del y
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del l
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del x
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del k
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del w
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del j
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del v
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del i
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del u
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del h
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del t
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del g
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del s
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del f
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del r
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del e
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del q
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del d
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del p
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del c
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
del o
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 7144 bytes in use; 107 refs in use
mem()
# output {"next": {"next": {"next": {...: ...}}}}
print(b)

del b
# output 7144 bytes in use; 107 refs in use
mem()
gc()
# output 3880 bytes in use; 59 refs in use
mem()
gc()
# output 3880 bytes in use; 59 refs in use
mem()
# output {"next": {"next": {"next": {...: ...}}}}
print(n)

del n
# output 3880 bytes in use; 59 refs in use
mem()
gc()
# output 344 bytes in use; 7 refs in use
mem()
# output {"next": {"next": {"next": {...: ...}}}}
print(a)

del a
# output 344 bytes in use; 7 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
