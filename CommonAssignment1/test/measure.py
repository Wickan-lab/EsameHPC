import os
import subprocess
import csv

with open('measures_log.csv', mode='w') as measures_file:
    fieldnames = ['rows', 'columns', 'threads', 'time']
    measures_writer = csv.writer(measures_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    n_measures = 1000

    n_threads_cases = ['0', '1', '2', '4', '8']

    problem_size_cases = [
        ['1000', '1000'],
        ['10000', '10000'],
        ['20000', '20000']
    ]

    measures_writer.writerow(fieldnames)
    
    for prbl_size in problem_size_cases:
        for tsks in n_threads_cases:
            print(f"Threads: {tsks}\tRows: {prbl_size[0]}\tColumns: {prbl_size[1]}")
            for _ in range(n_measures):
                command = ['../build/program']+prbl_size
                command.append(tsks)
                result = subprocess.run(command, stdout=subprocess.PIPE)

                row_to_write = command[1:]
                row_to_write.append(result.stdout.decode('utf-8').strip())

                measures_writer.writerow(row_to_write)