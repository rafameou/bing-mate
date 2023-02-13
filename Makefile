#______________________________________
# File:
#	Makefile
# Date:
#	10/08/2021
# Author:
#	rafameou
# Notes:
# Purpose:  
#_______________________________________

CC=gcc
CFLAGS=-Wall -Wextra -pedantic `pkg-config --cflags gtk+-3.0 libconfig libcurl json-c`
SRC_FILES=src/main.c src/gui.c 
INCLUDES=`pkg-config --libs gtk+-3.0 libconfig libcurl json-c`
TARGET=bing-mate
BUILD_DIR=bin
RM=rm -rf

LDD=ldd
AWK=awk

.PHONY: all clean libs
all:
	if [ ! -d "${BUILD_DIR}" ]; then mkdir $(BUILD_DIR); fi
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(BUILD_DIR)/$(TARGET) $(INCLUDES)

run:
	if [ ! -d "${BUILD_DIR}" ]; then mkdir $(BUILD_DIR); fi
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(BUILD_DIR)/$(TARGET) $(INCLUDES)
	$(BUILD_DIR)/$(TARGET)

libs:
#https://unix.stackexchange.com/a/85261
	if [ -d "${BUILD_DIR}" ]; then  ${LDD} $(BUILD_DIR)/$(TARGET) | ${AWK} 'NF == 4 {print $$3}; NF == 2 {print $$1}'; fi

clean:
	if [ -d "${BUILD_DIR}" ]; then ${RM} ${BUILD_DIR}; fi