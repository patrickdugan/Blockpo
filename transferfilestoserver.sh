#!/bin/bash

echo "Copying py files"
find . -name '*.py' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.68.251 'cd /home/lihki/GraphSettlement/Codes; cpio -i -vumd'
echo "Done"

echo "Copying pyc files"
find . -name '*.pyc' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.68.251 'cd /home/lihki/GraphSettlement/Codes; cpio -i -vumd'
echo "Done"

echo "Copying txt files"
find . -name '*.txt' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.68.251 'cd /home/lihki/GraphSettlement/Codes; cpio -i -vumd'
echo "Done"

echo "Copying class files"
find . -name '*.class' | cpio -o -Bav -H crc | sshpass -p 'lihki' ssh lihki@190.45.68.251 'cd /home/lihki/GraphSettlement/Codes; cpio -i -vumd'
echo "Done"