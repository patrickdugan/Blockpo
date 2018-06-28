#!/bin/bash

echo "Copying .cpp files"
find . -name '*.cpp' | cpio -pdm /media/lihki/Datos/GitHubTradelayer/omnicore-master
echo "Done"

echo "Copying .h files"
find . -name '*.h' | cpio -pdm /media/lihki/Datos/GitHubTradelayer/omnicore-master
echo "Done"

echo "Copying .sh files"
find . -name '*.sh' | cpio -pdm /media/lihki/Datos/GitHubTradelayer/omnicore-master
echo "Done"

echo "Copying .include files"
find . -name '*.include' | cpio -pdm /media/lihki/Datos/GitHubTradelayer/omnicore-master
echo "Done"

echo "Copying .ac files"
find . -name '*.ac' | cpio -pdm /media/lihki/Datos/GitHubTradelayer/omnicore-master
echo "Done"