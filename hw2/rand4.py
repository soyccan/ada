from random import randint as R

N = 7
print(N)

s = sorted([R(1,30) for _ in range(3)])
A = sorted([R(1,30) for _ in range(N)])

print(' '.join(map(str,s)))
print(' '.join(map(str,A)))
