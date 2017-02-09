#! /bin/bash
if [ -x mitar ]; then
   echo ""
else
   echo "El fichero no existe o no es ejecutable."
fi
if [ -d tmp ]; then
   rm -r tmp
fi 
mkdir tmp
cd tmp
touch fich1.txt
echo "Hola Mundo!" > fich1.txt
touch fich2.txt
head -10 /etc/passwd > fich2.txt
touch fich3.dat
head -c 1024 /dev/urandom > fich3.dat
../mitar -c -f fich.mtar fich1.txt fich2.txt fich3.dat
mkdir out
cp fich.mtar out
cd out
../../mitar -x -f fich.mtar
diff fich1.txt ../fich1.txt && diff fich2.txt ../fich2.txt && diff fich3.dat ../fich3.dat
if [ $? -eq 0 ]; then
   cd ../..
   echo "Correcto."
else 
   cd ../..
   echo "No es correcto."
fi
