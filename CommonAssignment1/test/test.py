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
	help="How many tests to run"
)

args = parser.parse_args()

def main():
    rows = 10
    cols = 10
    threads = 1

    for i in range(args.n){
        a,b = getStructs(rows,cols,a,b)
        result = a.dot(b)
        p_result = subprocess.run(['../src/main',rows,cols,threads]).returncode
        if result != p_result:
            print("Discrepancy !!!")
            print(a,b)
            print(result,p_result)
    }
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
