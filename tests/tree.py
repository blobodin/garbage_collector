# Example tree borrowed from https://upload.wikimedia.org/wikipedia/commons/f/f7/Binary_tree.svg

# -m 10000

left = {"value": 5, "children": []}
right = {"value": 11, "children": []}
right = {"value": 6, "children": [left, right]}
left = {"value": 2, "children": []}
left = {"value": 7, "children": [left, right]}
right = {"value": 4, "children": []}
right = {"value": 9, "children": [right]}
right = {"value": 5, "children": [right]}
tree = {"value": 2, "children": [left, right]}
# output {"children": [{"children": [..., ...], "value": 7}, {"children": [...], "value": 5}], "value": 2}
print(tree)
# output 3856 bytes in use; 75 refs in use
mem()

# No freeing should occur
del left
del right
# output 3856 bytes in use; 75 refs in use
mem()
# output {"children": [{"children": [..., ...], "value": 7}, {"children": [...], "value": 5}], "value": 2}
print(tree)

# Prune an inner node
del tree["children"][0]["children"][1]
# output 2600 bytes in use; 51 refs in use
mem()
# output {"children": [{"children": [], "value": 2}], "value": 7}
print(tree["children"][0])

# Move the root to its right child
tree = tree["children"][1]
# output 1336 bytes in use; 27 refs in use
mem()
# output 5 {"children": [{"children": [], "value": 4}], "value": 9}
print(tree["value"], tree["children"][0])

# Remove the tree
del tree
# output 72 bytes in use; 3 refs in use
mem()
