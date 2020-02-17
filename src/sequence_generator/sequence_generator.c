#include "index.h"
#include "sequence_generator.h"

/*
******************TO DO********************
1. Implement stationary process and (optional) Metropolis–Hastings algorithm
2. Implement varaible chord lenght to obtain irregular rythym
3. Link modules and write proper Makfile
4. Write project description
5. Estimate keys for generated sequence
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

int **initialize_transition_matrix()
{
    int **transition_matrix = calloc(NUM_OF_CHORDS, sizeof(int *));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        transition_matrix[i] = calloc(NUM_OF_CHORDS, sizeof(int));

    return transition_matrix;
}

char **initialize_chord_set()
{
    char **chord_set = calloc(NUM_OF_CHORDS, sizeof(char *));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        chord_set[i] = calloc(CHORD_LENGHT, sizeof(char));

    return chord_set;
}

void read_matrix(FILE *fp, char **chord_set, int **transition_matrix)
{
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        fscanf(fp, "%s", chord_set[i]);

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        for (int j = 0; j < NUM_OF_CHORDS; j++)
            fscanf(fp, "%d", &transition_matrix[i][j]);
}

void write_abc_header(FILE *abc, char note)
{
    fprintf(abc, "X: 1\n");
    fprintf(abc, "L: 1/8\n");
    fprintf(abc, "K: ");
    fprintf(abc, "%c\n", note);
}

int chord_label_id(int row, int column, int **transition_matrix)
{
    for (int i = 0; i < NUM_OF_CHORDS; i++)
    {
        if (i == 0 && column <= transition_matrix[row][i])
            return i;

        else if (i != 0 && transition_matrix[row][i - 1] > column && column <= transition_matrix[row][i])
            return i;
    }
    return -1;
}

void generate_sequence(FILE *fp, FILE *abc)
{
    int **transition_matrix = initialize_transition_matrix();
    char **chord_set = initialize_chord_set();
    read_matrix(fp, chord_set, transition_matrix);

    /* SEEDING NUMBER GENERATOR */
    pcg32_random_t rng;
    int rounds;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf,
                    (intptr_t)&rounds);

    int row, column;

    //PRIMARY STATE
    row = pcg32_boundedrand_r(&rng, NUM_OF_CHORDS);
    column = pcg32_boundedrand_r(&rng, transition_matrix[row][NUM_OF_CHORDS - 1]);

    int j = 0, id = 0;
    char note;

    while (chord_set[row][j] < 'A' || chord_set[row][j] > 'G')
        j++;

    note = chord_set[row][j];
    j = 0;

    write_abc_header(abc, note);

    while (j < 200)
    {
        id = chord_label_id(row, column, transition_matrix);
        fprintf(abc, "%s ", chord_set[row]);

        row = id;
        column = pcg32_boundedrand_r(&rng, transition_matrix[row][NUM_OF_CHORDS - 1]);
        j++;
    }

    clean_matrix((void **)chord_set, NUM_OF_CHORDS);
    clean_matrix((void **)transition_matrix, NUM_OF_CHORDS);
}

int main()
{
    FILE *fp = fopen("matrix.txt", "r");
    FILE *abc = fopen("a.abc", "w");

    generate_sequence(fp, abc);

    fclose(fp);
    fclose(abc);
    return 0;
}