# -m 10000

matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
# output [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
print(matrix)
# output 712 bytes in use; 20 refs in use
mem()

transpose = [None, None, None]
i = 0
while i < 3:
    transpose[i] = [0, 0, 0]
    i = i + 1

del i
# output [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
print(transpose)
# output 1352 bytes in use; 37 refs in use
mem()

i = 0
while i < len(matrix):
    row = matrix[i]
    j = 0
    while j < len(matrix):
        transpose[j][i] = row[j]
        j = j + 1
    i = i + 1
# output [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
print(transpose)
# output 1128 bytes in use; 30 refs in use
mem()

del matrix
del row
del i
del j
# output 712 bytes in use; 20 refs in use
mem()
# output [[1, 4, 7], [2, 5, 8], [3, 6, 9]]
print(transpose)
