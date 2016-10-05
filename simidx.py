#!/usr/bin/env python3
"""
    Copyright 2016-today
    Project cppsimhash

    Author: Steve Göring
"""
"""
    index based on simhash

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
import argparse
import glob
from multiprocessing import Pool
import multiprocessing
import subprocess

import simhashlib


def find(dirname):
    tmp = glob.glob(dirname + "/*")
    r = []
    for p in tmp:
        if os.path.isdir(p):
            r += find(p)
        else:
            r.append(p)
    return r


def read_simidx(idxfilename):
    f = open(idxfilename, "r")

    values = [x.strip().split(" : ") for x in f.readlines()]
    f.close()
    hashidx = {}
    for shash, doc in values:
        hashidx[int(shash)] = doc

    return hashidx


def calc_similarity(x, y):
    dist = simhashlib.hamming_distance(x, y)
    return 1 - dist / 64.0


def calc_simhash(filename):
    cmd = "./simhash {}".format(filename)
    print(filename + " done.")
    return simhashlib.simhash_file(filename)


def simidx_add_file(filename, simidxfile="_simidx"):
    shash = calc_simhash(filename)
    f = open(simidxfile, "a")
    f.write("{} : {}\n".format(shash, filename))
    f.close()


def simidx_add_dir(dirname, simidxfile="_simidx"):
    infiles = find(dirname)
    cpu_count = multiprocessing.cpu_count()
    pool = Pool(processes=cpu_count)
    res = pool.map(calc_simhash, infiles)
    f = open(simidxfile, "a")
    for shash, filename in zip(res, infiles):
        f.write("{} : {}\n".format(shash, filename))
    f.close()


def simidx_add(argsdict):
    if argsdict["c"] and os.path.isfile(argsdict["idx"]):
        os.remove(argsdict["idx"])

    for w in argsdict["what"]:
        if os.path.isfile(w):
            simidx_add_file(w, argsdict["idx"])
        else:
            simidx_add_dir(w, argsdict["idx"])


def simidx_query(argsdict):
    if not os.path.isfile(argsdict["querydoc"]):
        print("{} is not a file".format(argsdict["querydoc"]))
        return -1

    if not os.path.isfile(argsdict["idx"]):
        print("{} is not a file".format(argsdict["idx"]))
        return -1

    simidx = read_simidx(argsdict["idx"])

    # build hash of input document
    queryfilehash = calc_simhash(argsdict["querydoc"])

    print("hash of inputfile {} : {}".format(argsdict["querydoc"],queryfilehash))
    lowerBound = float(argsdict["lowerBound"])
    res = []
    for k in simidx:
        sim = calc_similarity(k, queryfilehash)
        if sim > lowerBound:
            res.append((sim, simidx[k]))

    print("most sim. documents (>{b}): {l}".format(l=len(res), b=lowerBound))
    for r in sorted(res, key=lambda x: x[0], reverse=True):
        print(r)


def main(params):
    parser = argparse.ArgumentParser(description='simidx', epilog="stg7 2016", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--idx', type=str, default="_simidx", help='sim index file')
    subparsers = parser.add_subparsers(help='sub-command', dest="cmd")

    parser_add = subparsers.add_parser('add', help='add a file to simidx', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser_add.add_argument('what', nargs="*", help='file or directory')
    parser_add.add_argument('-c', action='store_true', help='clean index file before adding new files')

    parser_query = subparsers.add_parser('query', help='query simindex', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser_query.add_argument('querydoc', help='document for query')
    parser_query.add_argument('--lowerBound', type=float, default=0.9, help='lower bound for similarity match')

    argsdict = vars(parser.parse_args())

    print(argsdict)

    if argsdict["cmd"] == "add":
        simidx_add(argsdict)
    if argsdict["cmd"] == "query":
        simidx_query(argsdict)


if __name__ == "__main__":
    main(sys.argv[1:])
