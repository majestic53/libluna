# libluna
# Copyright (C) 2015 David Jolly
# ----------------------
#
# libluna is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libluna is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -pthread -std=gnu++11 -O3 -Wall -Werror
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./include/
DIR_SRC=./src/
LIB=libluna.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)luna.o $(DIR_BUILD)luna_display.o $(DIR_BUILD)luna_exception.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: luna.o luna_display.o luna_exception.o

luna.o: $(DIR_SRC)luna.cpp $(DIR_INC)luna.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)luna.cpp -o $(DIR_BUILD)luna.o

luna_exception.o: $(DIR_SRC)luna_exception.cpp $(DIR_INC)luna_exception.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)luna_exception.cpp -o $(DIR_BUILD)luna_exception.o

# COMPONENTS

luna_display.o: $(DIR_SRC)luna_display.cpp $(DIR_INC)luna_display.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)luna_display.cpp -o $(DIR_BUILD)luna_display.o
