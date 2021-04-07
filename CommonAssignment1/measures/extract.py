#
# Copyright (C) 2021 - All Rights Reserved 
#
# This file is part of EsameHPC.
#
# EsameHPC is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# EsameHPC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with EsameHPC.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import numpy as np
import scipy as sp
from scipy import stats
import matplotlib.pyplot as plt
import csv
import pandas as pd
import logging

def _clean(filename):
	with open(filename,"r+") as f:
		d = f.readlines()
		f.seek(0)
		for i in d:
			if not ("Command" in i):
				f.write(i)
		f.truncate()

def _extract(path_to_folder,plot_columns):
	os.chdir(path_to_folder)
	filenames =  [f for f in os.listdir('.') if os.path.isfile(f)]
	if not os.path.exists("jpg"):
		os.mkdir("jpg")

	for filename in filenames:
		os.path.isdir(filename)
		if (filename.split(".")[-1] != "csv"):
			filenames.remove(filename)

	filenames = sorted(filenames)

	means = {}

	for filename in filenames:
		file_mean = {}
		print('Processing : ' + filename)
		_clean(filename)
		ds = pd.read_csv(filename)
		for col in plot_columns.keys():
			#extract the selected column
			x_data = ds[col]
			#plot data fitting a gaussian
			mean,std=stats.norm.fit(x_data)
			file_mean[col] = mean
			if not plot_columns[col]['jpg']:
				continue
			plt.hist(x_data, bins=200, density=True)
			xmin, xmax = plt.xlim()
			x = np.linspace(xmin, xmax, 100)
			y = stats.norm.pdf(x, mean, std)
			plt.plot(x, y)
			plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
			plt.close()
		means[filename] = file_mean
	return means

def _compute_speedup(t,tp,nt,psize):
	logging.info("-"*30)
	if tp == 0:
		logging.info("Speedup P = " + str(nt) + " & Problem Size = " + psize + "-> Divide By Zero")
		logging.info("Efficiency P = " + str(nt) + " & Problem Size = " + psize + "-> Divide By Zero")
	logging.info("Speedup P = " + str(nt) +  " & Problem Size = " + psize + " -> " + str(t/tp))
	logging.info("Efficiency P = " + str(nt) +  " & Problem Size = " + psize + " -> " + str(t/(tp*float(nt))))
	logging.info("-"*30)

def extraction(folder="measure/", cols={'elapsed':{'jpg':True,'speedup':True},'user':{'jpg':False,'speedup':False},'sys':{'jpg':False,'speedup':False}}, threads=[0,1,2,4,8]):
	logging.basicConfig(filename='extraction.log' ,level=logging.INFO, format='%(asctime)s %(message)s')
	means = _extract(folder,cols)
	logging.info("Problem size grows going towards the end of the file.\n All of the data was computed using square matrices")
	logging.info("Means : %s", means)
	#per calcolare lo speedup devo dividere il tempo elapsed sequenziale per il tempo parallelo elapsed
	nt_index = 0
	for filename_key in means:
		for col in cols:
			if not cols[col]['speedup']:
				continue
			if "NTH-0" in  filename_key:
				seq = means[filename_key][col]
			splitted_filename=filename_key.split("-")
			nt = splitted_filename[3]
			psize = splitted_filename[1]
			_compute_speedup(seq,means[filename_key][col],nt,psize)




if __name__ == "__main__":
	extraction()
