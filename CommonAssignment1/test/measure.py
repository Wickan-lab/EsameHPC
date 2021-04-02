import os
import subprocess
import csv

def main():
	n_measures = 10000
	fieldnames = ['rows', 'columns', 'threads', 'time']  
	n_threads_cases = ['0', '1', '2', '4', '8']
	problem_size_cases = [ ['10000', '10000'] ]
	results_to_print = []
	results_to_print.append(fieldnames)	

	for size in problem_size_cases:
		for threads in n_threads_cases:
			print(f"Threads: {threads}\tRows: {size[0]}\tColumns: {size[1]}")
			for _ in range(n_measures):
				command = ['../build/program'] + size
				command.append(threads)
				result = subprocess.run(command, stdout=subprocess.PIPE)

				row_to_write = command[1:]
				row_to_write.append(result.stdout.decode('utf-8').strip())
				results_to_print.append(row_to_write)

			with open('measures_log'+ time.strftime("%H-%M-%S") + '.csv', mode='a') as measures_file: 
				measures_writer = csv.writer(measures_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)  
				measures_writer.writerows(results_to_print)

			results_to_print = []

if __name__ == '__main__':
	main()