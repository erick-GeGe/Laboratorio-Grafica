import numpy as np

x = [[np.round(np.random.uniform(1,10),2) for _ in range(5)] for j in range(5)]
x = np.array(x)

print(x)