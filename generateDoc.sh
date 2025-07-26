#!/bin/bash
rm -rf html
rm -rf doc
doxygen Doxyfile
mv ./doc/html/* ./doc/
rm -rf ./doc/html
cd doc/latex
make
mv refman.pdf ../../documentation/
cd ../../documentation/
mv refman.pdf DocZappy.pdf
cd ..
rm -rf doc/
