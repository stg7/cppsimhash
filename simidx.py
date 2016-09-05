#!/usr/bin/env python3
import sys
import os
import argparse
import glob
from multiprocessing import Pool
import multiprocessing
import subprocess


def shell_call(call):
    """
    Run a program via system call and return stdout + stderr.
    @param call programm and command line parameter list, e.g "ls /"
    @return stdout and stderr of programm call
    """
    try:
        output = subprocess.check_output(call, stderr=subprocess.STDOUT, universal_newlines=True, shell=True)
    except Exception as e:
        output = str(e.output)
    return output

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
    get_bin = lambda x: format(x, 'b').zfill(64)

    bx = get_bin(x)
    by = get_bin(y)
    return 1 - sum(xx != yy for xx, yy in zip(bx, by)) / 64.0


def calc_simhash(filename):
    cmd = "./simhash {}".format(filename)
    print(filename + " done.")
    return shell_call(cmd).strip()


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
    queryfilehash = int(shell_call("./simhash {d}".format(d=argsdict["querydoc"])).strip())

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
