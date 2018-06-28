#!/bin/bash

echo "Doing Make"
./autogen.sh
./configure
make --jobs=8
echo "Done"
