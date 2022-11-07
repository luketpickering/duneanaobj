# Build outside of FNAL Stack like

```
mkdir build; cd build
cmake .. -DSTANDALONE_BUILD=ON -DCMAKE_INSTALL_PREFIX=/path/to/install/dir
make install
```

This will build and install the standard record dictionary and SRProxy class. Flat class on the way.