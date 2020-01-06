from random import randint as R

N = 80
print(N)
for i in range(N):
    for j in range(N):
        x = R(0,100)
        if x > 8:
            y = 1
        else:
            y = 0
        print(y, end='')
    print()
