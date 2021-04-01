import os
import sys
import subprocess
import numpy as np
import argparse

parser = argparse.ArgumentParser(
	description="Matrx Dot Vector"
)

parser.add_argument(
	"-tn",
	"--test-num",
	dest="n",
	default=1,
	type=int,
    required=True,
	help="How many tests to run"
)

parser.add_argument(
	"-c",
	"--cols",
	dest="cols",
	default=1,
	type=int,
    required=True,
	help="How many cols"
)

parser.add_argument(
	"-r",
	"--rows",
	dest="rows",
	default=1,
	type=int,
    required=True,
	help="How many rows"
)

args = parser.parse_args()

def main():
    rows = args.rows
    cols = args.cols
    threads = 1

    for i in range(args.n):
        a,b = getStructs(rows,cols)
        result = a.dot(b)
        subprocess.run(['./../src/main',str(rows),str(cols),str(threads),' > results.txt'])
        with open('results.txt','r'):
            lines = f.read().splitlines()
            p_result = np.array(lines,int)
            if numpy.array_equal(p_result,result):
                print("Discrepancy !!!")
                print(a,b)
                print(result,p_result)

    #a,b = getStructs(rows,cols)

def getStructs(rows,cols):
    a = []
    b = []
    for row_index in range(rows):
        r = [np.random.randint(10000) for i in range(cols)]
        a.append(r)

    a = np.array(a)
    b = np.array([np.random.randint(10000) for j in range(cols)])
    return a,b

if __name__ == '__main__':
    main()
