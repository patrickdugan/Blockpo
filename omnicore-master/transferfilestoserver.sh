#!/bin/bash

echo "Copying .cpp files"
find . -name '*.cpp' | cpio -o -Bav -H crc | sshpass -p 'Server1' ssh daniel@190.45.20.116 'cd /home/daniel/omnicore-masterlr/omnicore-master; cpio -i -vumd'
echo "Done"

echo "Copying .h files"
find . -name '*.h' | cpio -o -Bav -H crc | sshpass -p 'Server1' ssh daniel@190.45.20.116 'cd /home/daniel/omnicore-masterlr/omnicore-master; cpio -i -vumd'
echo "Done"

echo "Copying .sh files"
find . -name '*.sh' | cpio -o -Bav -H crc | sshpass -p 'Server1' ssh daniel@190.45.20.116 'cd /home/daniel/omnicore-masterlr/omnicore-master; cpio -i -vumd'
echo "Done"