#!/bin/bash

## Used to run executables.
if [ -z "$1" ]; then
  echo Usage: $0 executable logname \[arg1 arg2\]
  exit;
fi

export NAME=$1
shift

export LOG=${HOME}/scripts/${NAME}.log
export SAM=`whoami`.`date +%s`.${NAME}
touch $SAM
date > $LOG
sleep 1000
date >> $LOG
