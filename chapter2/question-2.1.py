#!"C:\Python33\python.exe"
from math import cos, pi

sample_rate = 10
frequency = 2
k = 1

for i in range(0, 4):
    t = i * 1 / sample_rate
    a = cos(2 * pi * frequency * t)
    b = cos(2 * pi * (k * sample_rate + frequency) * t)
    c = cos(2 * pi * (k * sample_rate - frequency) * t)
    print(a, b, c)

input()
