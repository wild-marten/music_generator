#include "matrix_generator.h"

/*
******************TO DO********************
1. Implement stationary process
*/

char **initialize_chord_set(int HEIGHT, int WIDTH)
{
    char **chord_set = calloc(HEIGHT, sizeof(char *));

    for (int i = 0; i < HEIGHT; i++)
        chord_set[i] = calloc(WIDTH, 1);

    return chord_set;
}

void read_chords(FILE *fp, char **chord_set)
{
    char *str = calloc(100, 1);
    int j = 0, flag = 0;

    while (fscanf(fp, "%s", str) == 1)
    {
        flag = 0;
        if (strcmp(str, "$") != 0)
        {
            for (int i = 0; i < j; i++)
                if (strcmp(str, chord_set[i]) == 0)
                {
                    flag = 1;
                    break;
                }

            if (flag == 0)
            {
                strcat(chord_set[j], str);
                j++;
            }
        }

        str = clean(str);
        str = calloc(100, 1);
    }

    // printf("%d", j);

    clean(str);
    fseek(fp, 0, SEEK_SET);
}

char **build_chord_table(FILE *fp)
{
    char **chord_set = initialize_chord_set(NUM_OF_CHORDS, CHORD_LENGHT);
    read_chords(fp, chord_set);

    return chord_set;
}

int find_chord_id(char **chord_set, char *chord)
{
    int j = 0;

    while (strcmp(chord_set[j], chord) != 0 && j < NUM_OF_CHORDS)
        j++;

    return j;
}

void write_matrix(char **chord_set, gsl_matrix *transition_matrix, FILE *result)
{
    gsl_matrix_fprintf(result, transition_matrix, "%f");

    fprintf(result, "\t");
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        fprintf(result, "%s ", chord_set[i]);
    fprintf(result, "\n");

}

void btm_occurences(FILE *fp, gsl_matrix *transition_matrix, char **chord_set)
{
    char *current_chord = calloc(CHORD_LENGHT, sizeof(char));
    char *next_chord = calloc(CHORD_LENGHT, sizeof(char));

    int crr_id = -1, nxt_id = -1;

    fscanf(fp, "%s", current_chord);

    while (1)
    {
        if (fscanf(fp, "%s", next_chord) != 1)
            break;

        if (strcmp(current_chord, "$") != 0 && strcmp(next_chord, "$") != 0)
        {
            if (crr_id == -1)
                crr_id = find_chord_id(chord_set, current_chord);

            nxt_id = find_chord_id(chord_set, next_chord);

            gsl_matrix_set(transition_matrix, crr_id, nxt_id, gsl_matrix_get(transition_matrix, crr_id, nxt_id)+1);
        }
        else
        {
            crr_id = -1;
            nxt_id = -1;
        }

        current_chord = clean(current_chord);
        current_chord = calloc(CHORD_LENGHT, sizeof(char));
        strcpy(current_chord, next_chord);
        crr_id = nxt_id;

        next_chord = clean(next_chord);
        next_chord = calloc(CHORD_LENGHT, sizeof(char));
    }

    clean(current_chord);
    clean(next_chord);
}

void build_transition_matrix(FILE *fp, FILE *result)
{
    gsl_matrix *transition_matrix = gsl_matrix_calloc(DIMX, DIMY);

    char **chord_set = build_chord_table(fp);

    btm_occurences(fp, transition_matrix, chord_set);

    write_matrix(chord_set, transition_matrix, result);

    gsl_matrix_free(transition_matrix);
    clean_matrix((void **)chord_set, NUM_OF_CHORDS);
}

int main()
{
    FILE *fp = fopen("build/txt_src/result.txt", "r");
    FILE *result = fopen("build/txt_src/matrix.txt", "w");

    build_transition_matrix(fp, result);

    fclose(fp);
    fclose(result);
    return 0;
}