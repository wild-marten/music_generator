#include "index.h"
#include "matrix_generator.h"

/*
******************TO DO********************
1. Implement stationary process
*/

void *clean(void *ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
        ptr = NULL;
    }
    return ptr;
}

void **clean_matrix(void **tab, int rows)
{
    for (int i = 0; i < rows; i++)
        if (tab[i] != 0)
            free(tab[i]);

    if (tab != 0)
        free(tab);
    return tab;
}

char **build_chord_table(FILE *fp)
{
    char **chord_set = calloc(NUM_OF_CHORDS, sizeof(char *));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        chord_set[i] = calloc(CHORD_LENGHT, 1);

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
    clean(str);
    fseek(fp, 0, SEEK_SET);
    return chord_set;
}

int **initialize_transition_matrix()
{
    int **transition_matrix = calloc(NUM_OF_CHORDS, sizeof(int *));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        transition_matrix[i] = calloc(NUM_OF_CHORDS, sizeof(int));

    return transition_matrix;
}

int find_chord_id(char **chord_set, char *chord)
{
    int j = 0;

    while (strcmp(chord_set[j], chord) != 0 && j < NUM_OF_CHORDS)
        j++;

    return j;
}

void print_matrix(char **chord_set, int **transition_matrix)
{
    printf("\t");

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        printf("%s ", chord_set[i]);
    printf("\n");

    for (int i = 0; i < NUM_OF_CHORDS; i++)
    {
        printf("%s ", chord_set[i]);
        for (int j = 0; j < NUM_OF_CHORDS; j++)
        {
            printf("%d ", transition_matrix[i][j]);
        }
        printf("\n");
    }
}

void write_matrix(char **chord_set, int **transition_matrix, FILE *result)
{
    fprintf(result, "\t");
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        fprintf(result, "%s ", chord_set[i]);
    fprintf(result, "\n");

    for (int i = 0; i < NUM_OF_CHORDS; i++)
    {
        for (int j = 0; j < NUM_OF_CHORDS; j++)
            fprintf(result, "%d ", transition_matrix[i][j]);
        fprintf(result, "\n");
    }
}


void btm_occurences(FILE *fp, int **transition_matrix, char **chord_set)
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

            transition_matrix[crr_id][nxt_id]++;
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

void btm_prefix_sums(int **transition_matrix)
{
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        for (int j = 0; j < NUM_OF_CHORDS; j++)
            if (j != 0)
                transition_matrix[i][j] += transition_matrix[i][j - 1];
}

void build_transition_matrix(FILE *fp, FILE *result)
{
    int **transition_matrix = initialize_transition_matrix();
    char **chord_set = build_chord_table(fp);

    btm_occurences(fp, transition_matrix, chord_set);
    // btm_prefix_sums(transition_matrix);

    write_matrix(chord_set, transition_matrix, result);
    clean_matrix((void **)transition_matrix, NUM_OF_CHORDS);
    clean_matrix((void **)chord_set, NUM_OF_CHORDS);
}

int main()
{
    FILE *fp = fopen("res.txt", "r");
    FILE *result = fopen("../matrix.txt", "w");
    build_transition_matrix(fp, result);

    fclose(fp);
    return 0;
}