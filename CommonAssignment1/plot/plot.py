import subprocess
import pkg_resources
import sys


required = {'numpy','scipy','matplotlib','csv'}
installed = {pkg.key for pkg in pkg_resources.working_set}
missing = required - installed
if missing:
	python = sys.executable
	subprocess.check_call([python, '-m', 'pip', 'install', *missing], stdout=subprocess.DEVNULL)

import numpy as np
import scipy as sp
from scipy import stats
import matplotlib.pyplot as plt
import csv

with open('../test/measures_log.csv','r') as csv_file:
	reader = csv.reader(csv_file)

for line in reader:
	print(line)

