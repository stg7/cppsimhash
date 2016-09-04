#!/usr/bin/env python3
import sys
import os
from ctypes import *

if not os.path.isfile("./libsimhash.so"):
    print("First run scons, so that ./libsimhash.so exists.")
    sys.exit(-1)

simhash_lib = cdll.LoadLibrary("./libsimhash.so")


def simhash(x):
    shash = simhash_lib.simhash
    shash.restype = c_ulong
    return shash(x)


def hamming_distance(x, y):
    shamming_distance = simhash_lib.hamming_distance
    shamming_distance.restype = c_int
    return shamming_distance(x, y)


def simhash_file(x):
    shashfile = simhash_lib.simhash_file
    shashfile.restype = c_ulong
    return shashfile(x)


if __name__ == "__main__":
    simhash_lib.bla()
    print(hamming_distance(1, 0))
    print(simhash_file("mars1.txt"))
    print(simhash("test"))
