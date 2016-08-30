#!/usr/bin/env python3
import sys
import os
import argparse



def main(params):
    parser = argparse.ArgumentParser(description='related query search', epilog="stg7 2016")
    #parser.add_argument('document', type=str, nargs='+', help='documents to process')
    argsdict = vars(parser.parse_args())

    """for doc in argsdict["document"]:
        simhash(doc)
    """

if __name__ == "__main__":
    main(sys.argv[1:])
