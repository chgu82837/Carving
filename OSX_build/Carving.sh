#!/bin/sh

BASEDIR=$(dirname $0)
PROJ_NAME="Carving"
cd $BASEDIR

# OBJ options:
# ../Carving/data/altair.obj
# ../Carving/data/Hot_Girl_01.obj
# ../Carving/data/Hulk.obj
# ../Carving/data/Luigi_obj.obj
# ../Carving/data/mario_obj.obj
# ../Carving/data/vanille_obj.obj

./$PROJ_NAME ../$PROJ_NAME/data/altair.obj ../$PROJ_NAME/data/vanille_obj.obj ../$PROJ_NAME/data/Hulk.obj
