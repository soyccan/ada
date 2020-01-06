import random, string, sys

la = random.randint(1, 10)
lb = random.randint(1, 10)
for _ in range(la):
    sys.stdout.write(random.choice('ab'))
sys.stdout.write("\n")
for _ in range(lb):
    sys.stdout.write(random.choice('ab'))
sys.stdout.write("\n")
