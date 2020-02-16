CC = gcc

BUILD = build/

CCFLAG = -Wall -Wextra -pedantic -std=c11 -Wfloat-equal -Winit-self -g3 -o0
SANITIZE = -Wuninitialized -fsanitize=leak -fno-common -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fstack-protector-all

analizer: analizer.c analizer.h index.h
	$(CC) $(CCFLAG) $(SANITIZE) -o $(BUILD)analizer analizer.c 
generator: generator.c
	$(CC) $(CCFLAG) $(SANITIZE) -o $(BUILD)generator generator.c 