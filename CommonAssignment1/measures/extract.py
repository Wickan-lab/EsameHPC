import os
import numpy as np
import scipy as sp
from scipy import stats
import matplotlib.pyplot as plt
import csv
import pandas as pd


os.chdir("5Kx5K_3000meas")
filenames = os.listdir()

for filename in filenames:
	if (filename.split(".")[-1] != "csv"):
		filenames.remove(filename)

filenames = sorted(filenames)

five_k_means = []

for filename in filenames:
	ds = pd.read_csv(filename)
	x_data = ds['time']
	mean,std=stats.norm.fit(x_data)
	five_k_means.append(mean)
	plt.hist(x_data, bins=200, density=True)
	xmin, xmax = plt.xlim()
	x = np.linspace(xmin, xmax, 100)
	y = stats.norm.pdf(x, mean, std)
	plt.plot(x, y)
	plt.savefig("jpg/" + filename.split('.')[0] + ".jpg")
	plt.close()

eight_k_means = []

os.chdir("../8Kx8K_3000meas")
filenames = os.listdir()

for filename in filenames:
        if (filename.split(".")[-1] != "csv"):
                filenames.remove(filename)

filenames = sorted(filenames)
print(filenames)


for filename in filenames:
        ds = pd.read_csv(filename)
        x_data = ds['time']
        mean,std=stats.norm.fit(x_data)
        eight_k_means.append(mean)
        plt.hist(x_data, bins=200, density=True)
        xmin, xmax = plt.xlim()
        x = np.linspace(xmin, xmax, 100)
        y = stats.norm.pdf(x, mean, std)
        plt.plot(x, y)
        plt.savefig("jpg/" + filename.split('.')[0] + ".jpg")
        plt.close()

print(five_k_means)

seq_five_time = five_k_means[0]
seq_eight_time = eight_k_means[0]

n_threads = ['0','1','2','4','8']
print("5000 elements ")
for i in range(1,len(five_k_means)):
	print("Speedup P = " + n_threads[i] + " -> " + str(seq_five_time/five_k_means[i]))

print("-"*100)
print(eight_k_means)

print("8000 elements")
for i in range(1,len(five_k_means)):
        print("Speedup P = " + n_threads[i] + " -> " + str(seq_eight_time/eight_k_means[i]))
