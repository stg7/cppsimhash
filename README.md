# cppsimhash

c++ simhash implementation for documents
and an additional (prototyp) simhash index for text documents

# Required
* python3
* scons
* g++ (c++14)
* cpu with hardware aes, `cat /proc/cpuinfo | grep "aes" | wc -l` should be > 0


# Build Steps
Just run `scons`


# Simidx -- usage
add a text document using `simidx.py`:
```
# add one document
./simidx.py add textfile

# add a folder
./simidx.py add textfolder

# after you created an index you can query it with
./simidx.py query <document.txt>
```

# Idea
For the approach and core idea have a look at papers in `doc`.

