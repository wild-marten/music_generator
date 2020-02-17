#ifndef ANAL
#define ANAL

#include"../index.h"

#define NUM_OF_SH 25
#define NUM_OF_INTERVALS 13
#define NUM_OF_CMP 6
#define NUM_OF_SEMITONES 12
#define NOTE_LENGHT 5


const char semitone[NUM_OF_SEMITONES][3] = {"C", "^C", "D", "^D", "E", "F", "^F", "G", "^G", "A", "^A", "B"};

const int interval[NUM_OF_INTERVALS] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21};


const char shorthand[NUM_OF_SH][10] = {"maj",     
                                 "min",     
                                 "dim",     
                                 "aug",     
                                 "maj7",    
                                 "min7",    
                                 "7",       
                                 "dim7",    
                                 "hdim7",   
                                 "minmaj7",
                                 "maj6", 
                                 "min6", 
                                 "9", 
                                 "maj9", 
                                 "min9",
                                 "sus4", 
                                 "sus2", 
                                 "11", 
                                 "maj11", 
                                 "min11",
                                 "13", 
                                 "maj13", 
                                 "min13", 
                                 "5", 
                                 "1"};

const int shorthand_components[NUM_OF_SH][NUM_OF_CMP] ={
{6, 10},                        //maj
{5, 10},                        //min
{5, 9},                         //dim
{6, 11},                        //aug
{6, 10, 14},                    //maj7
{5, 10, 13},                    //min7
{6, 10, 13},                    //7
{5, 9, 12},                     //dim7
{5, 9, 13},                     //hdim7
{5, 10, 14},                    //minmaj7
{6, 10, 12},                    //maj6
{5, 10, 12},                    //min6
{6, 10, 13, 18},                //9
{6, 10, 14, 18},                //maj9
{5, 10, 13, 18},                //min9
{8, 10},                        //sus4
{4, 10},                        //sus2
{6, 10, 13, 18, 22},            //11
{6, 10, 14, 18, 22},            //maj11
{5, 10, 13, 18, 22},            //min11
{6, 10, 13, 18, 22, 26},        //13
{6, 10, 14, 18, 22, 26},        //maj13
{5, 10, 13, 18, 22, 26},        //min13
{10},                           //5
{6, 10}                         //1
};                       
//intervals times two to keep it integer


int find_root_pos(char *chord);
bool is_mirex_chord(char *str);
void extract_chords(FILE *fp, FILE *result);
char *find_note(int root_position, int sh_cmp);
char *extract_shorthand(char *chord);
int shorthand_position(char *chord);
char *shorthand_to_notes(char *chord, char *abc_chord);
char *exeption(char *abc, char *chord, int cmp);
char *extensions_to_notes(char *chord, char *abc);
char *inversion(char *abc, char *chord);
char *translate_to_abc(char *chord);

#endif