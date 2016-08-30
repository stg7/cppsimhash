#!/usr/bin/env python3
import sys
import os
import argparse


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

def main(params):
    parser = argparse.ArgumentParser(description='simidx', epilog="stg7 2016")
    parser.add_argument('querydoc', type=str, help='search document')
    parser.add_argument('--idx', type=str, default="_simidx", help='sim index file')
    argsdict = vars(parser.parse_args())

    print(argsdict)
    if not os.path.isfile(argsdict["querydoc"]):
        print("{} is not a file".format(argsdict["querydoc"]))
        return -1

    if not os.path.isfile(argsdict["idx"]):
        print("{} is not a file".format(argsdict["idx"]))
        return -1

    simidx = read_simidx(argsdict["idx"])

    # build hash of input document
    os.system("cat {d} | ./simhash > {d}.hash".format(d=argsdict["querydoc"]))
    f = open(argsdict["querydoc"] + ".hash")
    queryfilehash = int(f.readline().strip())
    f.close()
    os.remove(argsdict["querydoc"] + ".hash")

    print(queryfilehash)
    lowerBound = 0.90
    res = []
    for k in simidx:
        sim = calc_similarity(k, queryfilehash)
        if sim > lowerBound:
            res.append((sim, simidx[k]))

    print("most sim. documents (>{b}): {l}".format(l=len(res), b=lowerBound))
    for r in sorted(res, key=lambda x: x[0], reverse=True):
        print(r)

if __name__ == "__main__":
    main(sys.argv[1:])
