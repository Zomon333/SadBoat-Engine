#!/bin/bash

#
#   Outputs the input from the command line and asks for a Y/N input.
#   Y or y continues the program
#   !(Y or y) stops the program
#   

echo -ne "\033[1;36m$1 (Y/N): \033[0m"
read -p "" -n 1 -r
echo " "

if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    exit 1
fi

echo " "
echo -e "\033[1;36mConfirmed; continuing\033[0m"
echo " "