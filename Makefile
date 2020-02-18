CC = gcc

BUILD = build/
SRC = src/
LIB = lib/

TXT_SRC = $(BUILD)txt_src/
OBJ = $(BUILD)obj/

ANAL = $(SRC)analizer/
PREP = $(SRC)prepare/
MG = $(SRC)matrix_generator/
SQ = $(SRC)sequence_generator/
RUN = $(SRC)run/

CCFLAG = -Wall -Wextra -pedantic -std=c11 -Wfloat-equal -Winit-self -g3 -o0
SANITIZE = -Wuninitialized -fsanitize=leak -fno-common -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fstack-protector-all
SLG = -lgsl -lgslcblas -lm

all: analizer matrix_generator sequence_generator prepare run

sequence_generator:
	$(CC) -Wall -I/usr/local/include -c $(SQ)sequence_generator.c -o $(OBJ)sequence_generator.o
	$(CC) $(CCFLAG) $(SANITIZE) -L/usr/local/lib $(OBJ)sequence_generator.o $(SLG) -o $(BUILD)sequence_generator

analizer: $(ANAL)analizer.c
	$(CC) $(CCFLAG) $(SANITIZE) -o $(BUILD)analizer $(ANAL)analizer.c

prepare:
	$(CC) $(CCFLAG) $(SANITIZE) -o prepare $(PREP)prepare.c

matrix_generator:
	$(CC) -Wall -I/usr/local/include -c $(MG)matrix_generator.c -o $(OBJ)matrix_generator.o
	$(CC) $(CCFLAG) $(SANITIZE) -L/usr/local/lib $(OBJ)matrix_generator.o $(SLG) -o $(BUILD)matrix_generator

run:
	$(CC) $(CCFLAG) $(SANITIZE) -o run $(RUN)run.c

clean:
	rm $(BUILD)analizer $(BUILD)matrix_generator $(BUILD)sequence_generator $(OBJ)matrix_generator.o $(OBJ)sequence_generator.o run prepare

clean_txt:
	rm $(TXT_SRC)*
