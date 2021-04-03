import os
import numpy as np
import scipy as sp
from scipy import stats
import matplotlib.pyplot as plt
import csv
import pandas as pd
import logging

def _clean(filename):
#	ds.drop(ds.index[("Command" in ds[col])],axis=0,inplace=True)
	with open(filename,"r+") as f:
		d = f.readlines()
		f.seek(0)
		for i in d:
			if not ("Command" in i):
				f.write(i)
		f.truncate()

def _extract(path_to_folder,plot_columns):
#	print("extract")
	os.chdir(path_to_folder)
	filenames = os.listdir()
	if not os.path.exists("jpg"):
		os.mkdir("jpg")

	for filename in filenames:
		if (filename.split(".")[-1] != "csv"):
			filenames.remove(filename)

	filenames = sorted(filenames)

	means = {}

	for filename in filenames:
		file_mean = {}
		print('Processing : ' + filename)
		_clean(filename)
		ds = pd.read_csv(filename)
		for col in plot_columns:
			#extract the selected column
			x_data = ds[col]
			#plot data fitting a gaussian
			mean,std=stats.norm.fit(x_data)
			plt.hist(x_data, bins=200, density=True)
			xmin, xmax = plt.xlim()
			x = np.linspace(xmin, xmax, 100)
			y = stats.norm.pdf(x, mean, std)
			plt.plot(x, y)
			plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
			plt.close()
			file_mean[col] = mean
		means[filename] = file_mean
	return means

def _compute_speedup(t,tp,nt):
	logging.info("-"*30)
	if tp == 0:
		logging.info("Speedup P = " + str(nt) + " -> Divide By Zero")
	logging.info("Speedup P = " + str(nt) + " -> " + str(t/tp))
	logging.info("-"*30)

def extraction(folder="measure/",cols=['elapsed'],threads=[0,1,2,4,8]):
#	print("GG")
	logging.basicConfig(filename='extraction.log' ,level=logging.INFO, format='%(asctime)s %(message)s')
	means = _extract(folder,cols)
	logging.info("Problem size grows going towards the end of the file")
	#per calcolare lo speedup devo dividere il tempo elapsed sequenziale per il tempo parallelo elapsed
	nt_index = 0
	for filename_key in means:
		nt_index += 1
		for col in cols:
			if "NTH-0" in  filename_key:
				seq = means[filename_key][col]
				nt_index = 0
			nt = filename_key.split("-")[1]
#			if nt_index != int(nt):
#				logging.warning("Possible file naming problem/Order Problem")
			_compute_speedup(seq,means[filename_key][col],nt)




if __name__ == "__main__":
	extraction()
