#!/bin/bash

echo "Doing Make"
./autogen.sh
./configure
make --jobs=3
echo "Done"
