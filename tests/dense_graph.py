# -m 10000

node_one   = {"name": "a"}
node_two   = {"name": "b"}
node_three = {"name": "c"}
node_four  = {"name": "d"}
node_five  = {"name": "e"}

adj = "adjacent"
node_one[adj] = [node_one, node_two, node_three, node_four, node_five]
node_two[adj] = [node_two, node_three, node_four, node_five, node_one]
node_three[adj] = [node_three, node_four, node_five, node_one, node_two]
node_four[adj] = [node_four, node_five, node_one, node_two, node_three]
node_five[adj] = [node_five, node_one, node_two, node_three, node_four]
# output {"name": "a", "adjacent": [{"name": "a", "adjacent": [..., ..., ..., ..., ...]}, {"name": "b", "adjacent": [..., ..., ..., ..., ...]}, {"name": "c", "adjacent": [..., ..., ..., ..., ...]}, {"name": "d", "adjacent": [..., ..., ..., ..., ...]}, {"name": "e", "adjacent": [..., ..., ..., ..., ...]}]}
print(node_one)
# output 2112 bytes in use; 39 refs in use
mem()
gc()
# output 2112 bytes in use; 39 refs in use
mem()
gc()
# output 2112 bytes in use; 39 refs in use
mem()
gc()
# output 2112 bytes in use; 39 refs in use
mem()
del node_two
del node_four
del node_five
del node_one
del node_three
# output 2112 bytes in use; 39 refs in use
mem()
gc()
# output 112 bytes in use; 4 refs in use
mem()
gc()
# output 112 bytes in use; 4 refs in use
mem()
gc()
# output 112 bytes in use; 4 refs in use
mem()

del adj
# output 72 bytes in use; 3 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
gc()
# output 72 bytes in use; 3 refs in use
mem()
