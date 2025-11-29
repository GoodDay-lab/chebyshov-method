import numpy as np
import pandas as pd

df = pd.read_csv("data/SLAU_var_8.csv", header=None)
A = df.values
n, _ = A.shape
A = A + np.eye(n)


def gen_opt_params(num_iter=128):
    opt = np.zeros(num_iter)
    opt[0] = 1.0

    cnt = 2
    while (cnt <= num_iter):
        for j in reversed(range(0, cnt)):
            if (j % 2 == 1):
                opt[j] = 2*cnt - opt[(j)//2]
            else:
                opt[j] = opt[j//2]
        cnt = cnt << 1

    opt = -np.cos((np.pi * opt / (2 * num_iter)))
    return opt


def check_spectre(matrix):
    minimum, maximum = 0, 0
    flag = False

    for i in range(matrix.shape[0]):
        centre = matrix[i, i]
        radius = 0
        for j in range(0, matrix.shape[1]):
            if (i == j):
                continue
            radius += np.abs(matrix[i, j])

        if (flag is False):
            minimum = centre - radius
            maximum = centre + radius
            flag = True

        minimum = min(centre - radius, minimum)
        maximum = max(centre + radius, maximum)
    return minimum, maximum


def train(mat, y, opts, spectre):
    tau0 = 2 / (spectre[0] + spectre[1])
    rho0 = (spectre[1] - spectre[0]) / (spectre[0] + spectre[1])

    x = np.zeros(mat.shape[1])

    print(opts)
    print(tau0 / (1 + rho0 * opts))

    for i in range(len(opts)):
        b = mat @ x
        param = tau0 / (1 + rho0 * opts[i])
        x = x + param * (y - b)

    return x


x_true = np.random.uniform(-1, 1, A.shape[1])
y_true = A @ x_true

x_pred = train(A, y_true, gen_opt_params(8), check_spectre(A))
print(x_pred)

print(check_spectre(A))
print(((x_pred - x_true)**2).mean())
