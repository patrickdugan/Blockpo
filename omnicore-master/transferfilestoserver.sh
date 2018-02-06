#!/bin/bash

echo "Copying .cpp files"
find . -name '*.cpp' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.20.116 'cd /home/lihki/omnicore-master; cpio -i -vumd'
echo "Done"

echo "Copying .h files"
find . -name '*.h' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.20.116 'cd /home/lihki/omnicore-master; cpio -i -vumd'
echo "Done"

echo "Copying .sh files"
find . -name '*.sh' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.20.116 'cd /home/lihki/omnicore-master; cpio -i -vumd'
echo "Done"

echo "Copying .include files"
find . -name '*.include' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.20.116 'cd /home/lihki/omnicore-master; cpio -i -vumd'
echo "Done"
