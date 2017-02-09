#!/bin/bash
MPOINT="./punto-montaje"
AUDITA=`uname -m`

if [ $AUDITA == "x86_64" ]; then
	AUDITA="../AuditaDisco/audita64"
else
	AUDITA="../AuditaDisco/audita"
fi

rm -R -f copiasTemporales
mkdir copiasTemporales
echo "Copiando fuselib.c en la carpeta temporal..."
cp src/fuseLib.c  copiasTemporales/
echo "Copiando fuselib.c en el FS..."
cp src/fuseLib.c $MPOINT/
echo "Copiando myFS.h en la carpeta temporal..."
cp src/myFS.h  copiasTemporales/
echo "Copiando myFS.h en el FS..."
cp src/myFS.h $MPOINT/
echo "Auditando el disco..."
$AUDITA 	../FUSE_miFS/disco-virtual
echo "Haciendo diff entre los ficheros originales y los copiados"
(diff src/fuseLib.c $MPOINT/fuseLib.c) && (diff src/myFS.h $MPOINT/myFS.h)
# fuseLib.c ocupa 5 bloques (20480 bytes). Después del truncate, fuseLib.c ocupa 4 bloques (16384 bytes).
echo "Truncando fuseLib.c en la carpeta temporal y en el FS de manera que ocupe un bloque de datos menos..."
truncate -s 16384 copiasTemporales/fuseLib.c
truncate -s 16384 $MPOINT/fuseLib.c
echo "Auditando el disco..."
$AUDITA 	../FUSE_miFS/disco-virtual
echo "Haciendo diff entre el fichero original fuseLib.c y el fichero truncado en el FS y la carpeta temporal..."
(diff src/fuseLib.c $MPOINT/fuseLib.c) && (diff src/fuseLib.c copiasTemporales/fuseLib.c)
echo "Copiando myFS.ch al FS..."
cp src/myFS.c $MPOINT/myFS.c
echo "Auditando el disco..."
$AUDITA 	../FUSE_miFS/disco-virtual
echo "Haciendo un diff entre el fichero original myFS.c y el fichero copiado en el FS..."
(diff src/myFS.c $MPOINT/myFS.c)
# myFS.h ocupa 1 bloque (4096 bytes). Después del truncate, myFS.h ocupa 3 bloques (12288 bytes).
echo "Truncando myFs.h en la carpeta temporal y en el FS de manera que ocupe unos bloques de datos más..."
truncate -s 12288 copiasTemporales/myFS.h
truncate -s 12288 $MPOINT/myFS.h
echo "Auditando el disco..."
$AUDITA 	../FUSE_miFS/disco-virtual
echo "Haciendo diff entre el fichero original myFS.h y el fichero truncado en el FS y la carpeta temporal..."
(diff src/myFS.h $MPOINT/myFS.h) && (diff src/myFS.h copiasTemporales/myFS.h)
