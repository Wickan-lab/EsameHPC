import os
import subprocess
import csv
import time

with open('test/measures_log'+ time.strftime("%H-%M-%S") + '.csv', mode='w') as measures_file:
    fieldnames = ['rows', 'columns', 'threads', 'time']
    measures_writer = csv.writer(measures_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    n_measures = 10000

    n_threads_cases = ['0', '1', '2', '4', '8']

    problem_size_cases = [
        ['10000', '10000'],
    ]

    measures_writer.writerow(fieldnames)
    
    for prbl_size in problem_size_cases:
        for tsks in n_threads_cases:
            results_to_print = []
            print(f"Threads: {tsks}\tRows: {prbl_size[0]}\tColumns: {prbl_size[1]}")
            for _ in range(n_measures):
                command = ['build/program.omp']+prbl_size
                command.append(tsks)
                result = subprocess.run(command, stdout=subprocess.PIPE)

                row_to_write = command[1:]
                row_to_write.append(result.stdout.decode('utf-8').strip())
                results_to_print.append(row_to_write)

            measures_writer.writerows(results_to_print)