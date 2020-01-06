from random import randint as R

N = 100000
M = 260
K = 50
print(N,M,K)
for _ in range(N):
    print(R(0,M-1), end=' ')
