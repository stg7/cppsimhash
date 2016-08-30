cppsimhash
==========

c++ simhash implementation for documents
and an additional (prototyp) simhash index for text documents

Required
---------
* python3
* scons
* g++ (c++14)

Build Steps
-----------
Just run scons


Simidx
------
add a text document using `simidx.py`:
```
# add one document
./simidx.py add textfile

# add a folder
./simidx.py add textfolder
```

Idea
----
For the approach and core idea have a look at papers in `doc`.

