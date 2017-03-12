#!/usr/bin/env python3
"""
    Copyright 2016-today
    Project cppsimhash

    Author: Steve Göring
"""
"""
    simhash python lib wrapper

    This file is part of cppsimhash.

    cppsimhash is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cppsimhash is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cppsimhash.  If not, see <http://www.gnu.org/licenses/>.
"""

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
