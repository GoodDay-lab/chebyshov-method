import argparse
import json
import os
import subprocess

import matplotlib.pyplot as plt
import numpy as np


parser = argparse.ArgumentParser(
    prog="analyze.py",
    description="Программа для построения графиков и анализа работы программы"
)
parser.add_argument('program', help="Путь к программе")
parser.add_argument('matrix', help="Путь к файлу с матрицей")
args = parser.parse_args()

os.system("make rebuild")

N_iters = [8, 16, 32, 64, 128, 256, 512]
M_type = ["one-step", "two-step"]

for label in M_type:

    errors = []
    print(f"Running {label} method")
    for n_iter in N_iters:
        process = subprocess.run([args.program, args.matrix, str(n_iter), label],
                                 capture_output=True, text=True)
        results = json.loads(process.stdout)

        f1 = np.array(results['x_orig']['data'])
        f2 = np.array(results['x_pred']['data'])

        errors.append(np.sqrt(((f1 - f2)**2).sum()))

    print(errors)
    plt.title("Ошибка невязки")
    plt.plot(list(range(len(errors))), errors)
    plt.show()
