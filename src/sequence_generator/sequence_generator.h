#ifndef SQ
#define SQ

#include "../index.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <sys/time.h>

#define NUM_OF_CMP 7
#define NUM_OF_INTERVALS 13
#define NUM_OF_SCALES 7
#define NUM_OF_SEMITONES 12
#define NOTE_LENGHT 5

const char semitone[NUM_OF_SEMITONES][3] = {"C", "^C", "D", "^D", "E", "F", "^F", "G", "^G", "A", "^A", "B"};

const int interval[NUM_OF_INTERVALS] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21};


int scale_cmp[NUM_OF_SCALES][NUM_OF_CMP] = {
    {2, 4, 6, 8, 10, 12, 14}, //Major
    {2, 4, 5, 8, 10, 11, 13}, //Minor
    {2, 4, 6, 8, 10, 12, 13}, //Mixolydian
    {2, 4, 5, 8, 10, 12, 13}, //Dorian
    {2, 3, 5, 8, 10, 11, 13}, //Phrygian
    {2, 4, 6, 9, 11, 12, 14}, //Lydian
    {2, 3, 5, 8, 9, 11, 13}   //Locrian
};

char scales[NUM_OF_SCALES][CHORD_LENGHT] = {
                                               "",
                                               "m",
                                               "Mix",
                                               "Dor",
                                               "Phr",
                                               "Lyd",
                                               "Loc"};



void *clean(void *ptr);
void **clean_matrix(void **tab, int rows);
double **initialize_transition_matrix();
char **initialize_chord_set();
void read_matrix(FILE *matrix, double **transition_matrix);
void read_chord_set(FILE *chs, char **chord_set);
double *get_stat_dis(FILE *sd);
void write_abc_header(FILE *abc);
int chord_id(int row, int column, int **transition_matrix);
gsl_rng *set_rng();
char *find_note(int root_position, int sh_cmp);
char *note(char *chord, int j);
int position(char *note);
int *scale_ids(char *chord);
char *sscale(char *chord);
bool validation(int x0, int x1, double *stat_dis, double **transition_matrix);
void generate_sequence(FILE *matrix, FILE *abc, FILE *chs, FILE *sd);

#endif