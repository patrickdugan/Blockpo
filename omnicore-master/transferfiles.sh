#!/bin/bash

host='daniel@190.45.20.116'
directory='/home/daniel/omnicore-master'

echo "Remotely copying"
scp -v * $host:$directory
echo "Done"
