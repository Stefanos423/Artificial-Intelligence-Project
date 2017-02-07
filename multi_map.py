#! /usr/bin/env python
import sys
import os
import subprocess
import time

N = 4
if not os.path.isdir("maps"):
    os.mkdir("maps")

confs = (("maps/variable size", range(10, 50, 2) + range(50, 1001, 40), [5], [10]),
        ("maps/variable density, small map", [20],[5], range(0, 31, 1)),
        ("maps/variable density, large map", [200], [5], range(0, 31, 1)),
        ("maps/variable checkpoints, large map", [300], range(1, 20, 1) + range(20, 101, 4), [10]))

for i in range(N):
    for conf in confs:
        time.sleep(1)
        if not os.path.isdir(conf[0]):
            os.mkdir(conf[0])
            
        for item1 in conf[1]:
            for item2 in conf[2]:
                for item3 in conf[3]:
                        filename = "{}/map_{}_{}_{}_{}.txt".format(conf[0], item1, item2, item3 / 100.0, i + 1)
                        subprocess.call(["./gen", filename, str(item1), str(item1), str(item2), str(item3 / 100.0), "1"])

                