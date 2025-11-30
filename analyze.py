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


def plot_errors_with_min(iterations, errors_x, errors_b, label):
    plt.figure(figsize=(14, 6))

    min_x = min(errors_x)
    min_b = min(errors_b)
    min_x_iter = iterations[errors_x.index(min_x)]
    min_b_iter = iterations[errors_b.index(min_b)]

    plt.loglog(iterations, errors_x, label=f'Ошибка по x (min: {min_x})', 
             marker='o', markersize=4, color='blue')
    plt.loglog(iterations, errors_b, label=f'Ошибка по b (min: {min_b})', 
             marker='s', markersize=4, color='red')

    plt.scatter(min_x_iter, min_x, color='blue', s=100, zorder=5, 
                edgecolors='black', linewidth=2, label=f'Min x (итер. {min_x_iter})')
    plt.scatter(min_b_iter, min_b, color='red', s=100, zorder=5, 
                edgecolors='black', linewidth=2, label=f'Min b (итер. {min_b_iter})')

    plt.xticks(iterations, [str(i) for i in iterations])
    plt.xlabel('Количество итераций')
    plt.ylabel('Ошибка')
    plt.title(f'Сходимость метода решения СЛАУ ({label})')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig(f"Figure_{label}.png")


for label in M_type:

    errors_x = []
    errors_b = []
    print(f"Running {label} method")
    for n_iter in N_iters:
        process = subprocess.run([args.program, args.matrix, str(n_iter), label],
                                 capture_output=True, text=True)
        results = json.loads(process.stdout)

        f1 = np.array(results['x_orig']['data'])
        f2 = np.array(results['x_pred']['data'])

        b1 = np.array(results['b_orig']['data'])
        b2 = np.array(results['b_pred']['data'])

        errors_x.append(np.sqrt(((f1 - f2)**2).sum()))
        errors_b.append(np.sqrt(((b1 - b2)**2).sum()))

    plot_errors_with_min(N_iters, errors_x, errors_b, label)
