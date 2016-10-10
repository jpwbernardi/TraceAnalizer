#!/bin/bash
mkdir $HOME/.bonnmotion
cd $HOME/.bonnmotion
wget http://sys.cs.uos.de/bonnmotion/src/bonnmotion-3.0.1.zip
unzip bonnmotion-3.0.1.zip -d ./
cd bonnmotion-3.0.1
./install
echo "PATH=\"\$HOME/.bonnmotion/bonnmotion-3.0.1/bin:\$PATH\"" > tmp
cat tmp >> ~/.profile
rm tmp ../bonnmotion-3.0.1.zip
printf "\nYou need to restart the computer to use bonnmotion\n"
