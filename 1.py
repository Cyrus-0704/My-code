a, n = map(int, input().split())

total = 0
current_term = 0

for i in range(1, n + 1):
    current_term = current_term * 10 + a
    total += current_term

print(total)    