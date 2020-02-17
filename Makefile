CC = gcc

BUILD = build/
OBJ = $(BUILD)obj/
SRC = src/
LIB = lib/
PCG = $(LIB)pcg-c-basic-0.9/
TXT_SRC = $(BUILD)/txt_src/
ANAL = $(SRC)/analizer
EXEC = $(SRC)/execute
MG = $(SRC)/matrix_generator
SQ = $(SRC)/sequence_generator
TEST = tests/

CCFLAG = -Wall -Wextra -pedantic -std=c11 -Wfloat-equal -Winit-self -g3 -o0
SANITIZE = -Wuninitialized -fsanitize=leak -fno-common -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fstack-protector-all
SLG = -lgsl -lgslcblas -lm

all: analizer matrix_generator sequence_generator

sequence_generator:
	$(CC) $(CCFLAG) $(SANITIZE) -o $(BUILD)sequence_generator $(PCG)pcg_basic.o $(SQ)sequence_generator.c

test:
	$(CC) -Wall -I/usr/local/include -c $(TEST)test.c -o $(OBJ)test.o
	$(CC) $(CCFLAG) $(SANITIZE) -L/usr/local/lib $(OBJ)test.o $(SLG) -o $(BUILD)test

analizer:
	$(CC) $(CCFLAG) $(SANITIZE) -o $(BUILD)analizer $(ANAL)analizer.c

matrix_generator: matrix_generator.c
	$(CC) $(CCFLAG) $(SANITIZE) -o $(BUILD)matrix_generator $(MG)matrix_generator.c

clean:
	rm $(OBJ)*
	rm $(BUILD)*

clean_txt:
	rm $(TXT_SRC)*
