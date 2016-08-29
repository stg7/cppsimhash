# -*- coding: utf8 -*-

import os
import sys
import glob
import multiprocessing

def terminal_width():
    rows, columns = os.popen('stty size', 'r').read().split()
    return int(columns)

def colorgreen(m):
    return "\033[92m" + m + "\033[0m"

def lInfo(msg):
    print(colorgreen("[INFO ] ") + str(msg))

def line():
    print("-" * terminal_width())

def dline():
    print("=" * terminal_width())

SetOption('num_jobs', multiprocessing.cpu_count()) # build with all aviable cpu cores/threads

line()
lInfo("build target(s): " + str(map(str, BUILD_TARGETS)))
lInfo("run with cpus  : " + str(multiprocessing.cpu_count()))
line()


env = Environment(CPPPATH = ["src/"])

env.Append(LINKFLAGS=['-pthread'])

env.Decider('MD5')

conf = Configure(env)

needed_libs = ['stdc++fs']


for lib in needed_libs:
    if not conf.CheckLib(lib, language="c++"):
        print "Unable to find lib: " + lib + ". Exiting."
        exit(-1)

env.Append(CXXFLAGS=['-std=c++14'])

# if you call scons debug=1 debug build is activated
if ARGUMENTS.get('debug', 0) != 0:
    # more checks
    env.Append(CXXFLAGS=['-Wall','-pedantic-errors', '-g', '-march=native', '-maes'])
    # "no" optimization
    env.Append(CXXFLAGS=['-O0'])
else:
    env.Append(CXXFLAGS=['-march=native', '-maes']) # use native architecture
    #env.Append(CXXFLAGS=['-mavx'])
    env.Append(CXXFLAGS=['-Wall'])
    env.Append(CXXFLAGS=['-O3'])

    # loop unrolling and link time optimization, options should be tested
    env.Append(CXXFLAGS=['-funroll-loops', '-flto', '-fwhole-program'])


header = set(glob.glob("src/*.hpp") +  glob.glob("src/*/*.hpp"))
sources = set(glob.glob("src/*.cpp") + glob.glob("src/*/*.cpp")) - set(["src/simhash.cpp"])


simhash = env.Program('simhash', ["src/simhash.cpp"] + list(sources))

Default(simhash)