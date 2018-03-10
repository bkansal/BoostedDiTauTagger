#!/bin/bash

#DELPHES_PATH="/opt/delphes"
DELPHES_PATH="/opt/Delphes-3.4.1"

INPUT_FILE=$1
OUTPUT_FILE=$2

# Check syntax
if [ -z "${INPUT_FILE// }" ] || [ -z "${OUTPUT_FILE// }" ]; then
    echo "Syntax: runDelphes.sh <input> <output>"
    exit
fi

# Check if input exists
if [ ! -f $INPUT_FILE ]; then
    echo "Cannot find input file:" $INPUT_FILE
    exit
fi

INPUT_FILE=`pwd`/$INPUT_FILE
OUTPUT_FILE=`pwd`/$OUTPUT_FILE

echo "Input:" $INPUT_FILE
echo "Output:" $OUTPUT_FILE

rm -f $OUTPUT_FILE

cd $DELPHES_PATH
./DelphesHepMC cards/delphes_card_CMS.tcl $OUTPUT_FILE $INPUT_FILE

rm $INPUT_FILE
