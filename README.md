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
add a text document using `add_to_simidx.sh`:
```
# add one document
./add_to_simidx.sh text.txt   

# add a folder
./add_to_simidx.sh textfolder
```


