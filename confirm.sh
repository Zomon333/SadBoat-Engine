#!/bin/bash
echo -ne "\033[1;36mMesa3D requires your apt and apt-get settings to allow source code downloads. Are these enabled? (Y/N): \033[0m"
read -p "" -n 1 -r
echo " "

if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    exit 1
fi

echo " "
echo -e "\033[1;36mConfirmed; continuing\033[0m"
echo " "