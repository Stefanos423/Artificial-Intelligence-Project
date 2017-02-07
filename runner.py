#! /usr/bin/env python
import sys
import os
import subprocess
import shutil

N = 4
if not os.path.isdir("maps"):
    os.mkdir("maps")

confs = (("maps/variable size", range(10, 50, 2) + range(50, 1001, 40), [5], [10]),
        ("maps/variable density, small map", [20],[5], range(0, 31, 1)),
        ("maps/variable density, large map", [200], [5], range(0, 31, 1)),
        ("maps/variable checkpoints, large map", [300], range(1, 20, 1) + range(20, 101, 4), [10]))

for conf in confs:
    if not os.path.isdir(conf[0]):
        os.mkdir(conf[0])
        
    for item1 in conf[1]:
        for item2 in conf[2]:
            for item3 in conf[3]:
                sys.stdout.write("{}\t{}\t{}\t".format(item1, item2, item3 / 100.0))
                for i in range(N):
                    filename = "{}/map_{}_{}_{}_{}.txt".format(conf[0], item1, item2, item3 / 100.0, i + 1)
                    shutil.copyfile(filename, "input.txt")
                    result = subprocess.check_output(["/usr/bin/time -f %e ./ai 2>&1"], shell=True)
                    num_lines1 = sum(1 for line in open('robot1.txt') if line.rstrip())
                    num_lines2 = sum(1 for line in open('robot2.txt') if line.rstrip())
                    sys.stdout.write("{}\t{}\t{}\t".format(result.strip(), num_lines1, num_lines2))
                    sys.stdout.flush()
                    
                sys.stdout.write("\n")
    sys.stdout.write("\n")         

                