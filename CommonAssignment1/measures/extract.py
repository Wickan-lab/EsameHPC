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
import matplotlib.pyplot as plt
import pandas as pd
import logging
from scipy import stats
from prettytable import PrettyTable
from prettytable import MARKDOWN

config = {
			'init':{

				'jpg':False,
				'speedup':False
				
			},
			'dotprod':{

				'jpg':False,
				'speedup':False

			},
			'user':{

				'jpg':False,
				'speedup':False

				},
			'sys':{

				'jpg':False,
				'speedup':False

				},
			'elapsed':{

				'jpg':True,
				'speedup':True

				}
		}

def _clean(filename):
	with open(filename,"r+") as f:
		d = f.readlines()
		f.seek(0)
		for i in d:
			if not ("Command" in i): # TODO : use regex instead, and print in log something to signal it
				f.write(i)
		f.truncate()

def _extract(path_to_folder,plot_columns):
	prev = os.getcwd()
	os.chdir(path_to_folder)

	#List diresctory
	filenames =  [f for f in os.listdir('.') if os.path.isfile(f)]
	if not os.path.exists("jpg"):
		os.mkdir("jpg")

	#Remove not csv files
	for filename in filenames:
		os.path.isdir(filename)
		if (filename.split(".")[-1] != "csv"):
			filenames.remove(filename)

	filenames = sorted(filenames)
	means = {}
	
	for filename in filenames:
		file_mean = {}
		print('Processing : ' + filename)
		ds = pd.read_csv(filename)
		for col in plot_columns.keys():
			print('Processing : ' + filename + ", Col : " + col)
			#extract the selected column
			x_data = ds[col]
			mean,std=stats.norm.fit(x_data)
			#68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
			#x_data = ds[ds[col] < (mean + std)]
			#x_data = ds[ds[col] > (mean - std)]

			file_mean[col] = mean
			
			if plot_columns[col]['jpg']:	
				plt.hist(x_data, bins=20, density=True)
				#xmin, xmax = plt.xlim()
				#x = np.linspace(xmin, xmax, 100)
				#y = stats.norm.pdf(x, mean, std)
				#plt.plot(x, y)
				plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
				plt.close()
			
		means[filename] = file_mean
	os.chdir(prev)
	return means

def _compute_speedup(t,tp,nt,psize):
	speedup = t/tp
	efficiency = t/(tp*float(nt))
	if tp == 0:
		logging.info("Speedup P = " + str(nt) + " & Problem Size = " + psize + "-> Divide By Zero")
		logging.info("Efficiency P = " + str(nt) + " & Problem Size = " + psize + "-> Divide By Zero")
	return speedup,efficiency

def _make_table(header,rows,print_table=False,save=True,name=""):
	if save and not name:
		raise Exception("No filename to save file")
	x = PrettyTable()
	x.field_names = header
	x.add_rows(rows)
	if save:
		_save_table(x,name)
	if print_table:
		print(x)
	return x

def _save_table(table,filename):
	with open(filename,"w") as table_file:
		table.set_style(MARKDOWN)
		data = table.get_string()
		table_file.write(data)

def _plot_from_table(header,rows,save=True,name="",show_plot=False):
	if save and not name:
		raise Exception("No filename to save file")

	x = [0]
	y = [0]
	speedup_pos = header.index("Speedup")
	thread_pos = header.index("Threads")
	for row in rows[1:]:
		x.append(row[thread_pos])
		y.append(row[speedup_pos])

	x_th = np.linspace(0, 4.8, 5)
	plt.style.use('seaborn-whitegrid')
	plt.autoscale(enable=True, axis='x', tight=True)
	plt.autoscale(enable=True, axis='y', tight=True)		
	plt.plot(x,y,'r',label='Measured')
	plt.plot(x_th,x_th + 0,'b',label='Ideal')
	plt.legend()
	plt.xlabel("Processors")
	plt.ylabel("Speedup")
	if show_plot:
		plt.show()
	if save:
		plt.savefig(name)
	plt.close()

def extraction(root="measure/", cols=config, threads=[0,1,2,4,8]):
	print("Initializing logger")
	logging.basicConfig(filename='extraction.log' ,level=logging.INFO, format='%(asctime)s %(message)s')
	print("Listing folder for problem size")
	folders =  [f for f in os.listdir(root) if os.path.isdir(os.path.join(root,f))]
	print(f"Found folders : {folders}")

	for folder in folders:
		print(f"Folder : {folder}")
		joined_path = os.path.join(root,folder)
		means = _extract(joined_path,cols)
		header = {'values':['Version','Threads','User','Sys','Elapsed','Speedup','Efficiency']}
		cells = {'values':[]}
		nt = -1
		for filename_key in means:
			cell = []
			splitted_filename=filename_key.split("-")
			if "NTH-0" in filename_key:
				seq = means[filename_key]['elapsed']
				nt = 1
				cell.append('Serial')
				cell.append(nt)
			else:
				nt = splitted_filename[3]
				cell.append('Parallel')
				cell.append(nt)

			for col in cols:
				cell.append(means[filename_key][col])
				if cols[col]['speedup']:
					psize = splitted_filename[1]
					speedup,efficiency = _compute_speedup(seq,means[filename_key][col],nt,psize)
					cell.append(speedup)
					cell.append(efficiency)
			cells['values'].append(cell)
		
		splitted_folder = folder.split("-")
		size = splitted_folder[1]
		opt = splitted_folder[2]
		table_filename = joined_path + "/psize-" + size + "-" + str(opt) + "-table.md"
		plot_filename = joined_path + "/speedup-" + str(size) + "-" + str(opt) +  ".jpg"

		table = _make_table(header['values'],cells['values'],name=table_filename)
		_plot_from_table(header["values"],cells["values"],name=plot_filename)

if __name__ == "__main__":
	extraction()
