import numpy as np

x = np.arange(10)
x.flags.freeze_on_view=1
print(x)
x[1:] = x[1:] * 2
y = x[1:]
print(y)
try:
    y[0] = 1
except ValueError as e:
    print("failed as expected:", e)
else:
    print("unexpectedly succeeded")

try:
    x[0] = 1
except ValueError as e:
    print("failed as expected:", e)
else:
    print("unexpectedly succeeded")




