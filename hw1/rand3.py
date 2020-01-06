from random import shuffle

a = list(range(1, 5000))
shuffle(a)
print(len(a))
print(' '.join(map(str, a)))
