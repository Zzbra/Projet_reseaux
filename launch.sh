#!/bin/bash
# launch.sh

cd VM1
echo "Démarrage VM1"
vagrant up

cd ../
cd VM2
echo "Démarrage VM2"
vagrant up

cd ../
cd VM3
echo "Démarrage VM3"
vagrant up

cd ../
cd VM1-6
echo "Démarrage VM1-6"
vagrant up


cd ../
cd VM3-6
echo "Démarrage VM3-6"
vagrant up
