# -m 14352

z = {"next": 1}
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
# output {"next": {"next": {"next": {...: ...}}}}
print(a)
# output 7176 bytes in use; 108 refs in use
mem()
del z
del y
del x
del w
del v
del u
del t
del s
del r
del q
del p
del o
del n
del m
del l
del k
del j
del i
del h
del g
del f
del e
del d
del c
del b
# output {"next": {"next": {"next": {...: ...}}}}
print(a)
# output 7176 bytes in use; 108 refs in use
mem()
del a
# output 72 bytes in use; 3 refs in use
mem()
