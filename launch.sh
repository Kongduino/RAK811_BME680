#!/bin/sh
PGM=`find ~/Library/Arduino15/packages/STMicroelectronics/tools/STM32Tools/ -name stm32CubeProg.sh`
BIN=`ls *bin`
echo "$PGM 0 $BIN -g"
sh $PGM 0 $BIN -g
