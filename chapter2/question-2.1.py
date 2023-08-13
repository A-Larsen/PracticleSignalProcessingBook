#!"C:\Python33\python.exe"
from math import cos, pi

sample_rate = 10
frequency = 2

for sample_number in range(0, 40):
    for k in range(-20, 20):
        t = sample_number * 1 / sample_rate
        a = cos(2 * pi * frequency * t)
        b = cos(2 * pi * (k * sample_rate + frequency) * t)
        c = cos(2 * pi * (k * sample_rate - frequency) * t)
        print(a, b, c)
