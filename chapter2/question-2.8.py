i = 0

rates = (300, 200, 100, 30)
for sample_rate in rates:
    print(f"rates {rates[i]}")
    for k in range(0, 5):
        for f in range(120, 130):
            print(sample_rate * k + f)
    i += 1
