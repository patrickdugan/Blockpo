#!/bin/bash

echo "Doing Make"
./autogen.sh
./configure
make --jobs=10
echo "Done"
