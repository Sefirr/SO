#!/bin/bash

MPOINT="./punto-montaje"

rm -R -f test
mkdir test

echo 'fichero 1' > ./test/fichero1.txt
echo "Copiando fichero 1"
cp ./test/fichero1.txt $MPOINT/
read -p "Press enter..."

echo "Creando fichero 2"
echo 'este es el fichero 2' > $MPOINT/fichero2.txt
ls $MPOINT -la
read -p "Press enter..."

