from random import randint as R

N = 200000
M = 500000
Q = 200000
print(N,M,Q)
for i in range(M):
    u = R(1,N)
    v = R(1,N)
    if u > v: u,v=v,u
    print(u,v)
for i in range(Q):
    print(R(1,N),R(1,N))
