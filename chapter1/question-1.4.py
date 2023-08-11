import cmath
tau = 2 * cmath.pi

def average(readings):
    # radians = degrees * (pi / 180)
    base = cmath.e ** (1j * tau / 360)
    total = 0
    for r in readings:
        total += base ** r
    result = total / len(readings)
    return cmath.log(result, base).real

a = average((12, 15, 13, 9, 16))

print(a)
input()

