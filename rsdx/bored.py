from string import ascii_lowercase
from itertools import product

A = [''.join(p) for p in product(ascii_lowercase, repeat = 2)]
B = [*filter(lambda x: 'b' not in x, A)]
print(len(A), len(A) - len(B))
