#include "matrix_generator.h"

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

    tab = clean(tab);
    return tab;
}

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

void write_chord_set(char **chord_set, FILE *chs)
{
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        fprintf(chs, "%s\n", chord_set[i]);
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

            gsl_matrix_set(transition_matrix, crr_id, nxt_id, gsl_matrix_get(transition_matrix, crr_id, nxt_id) + 1);
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

double *btm_sums(gsl_matrix *transition_matrix)
{
    double *sums = calloc(NUM_OF_CHORDS, sizeof(double));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
    {
        for (int j = 0; j < NUM_OF_CHORDS; j++)
            sums[i] += gsl_matrix_get(transition_matrix, i, j);
    }

    return sums;
}

void btm_probability(gsl_matrix *transition_matrix, double *sums)
{
    double value = 0.0;

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        for (int j = 0; j < NUM_OF_CHORDS; j++)
        {
            if (sums[i] != 0)
            {
                value = gsl_matrix_get(transition_matrix, i, j) / sums[i];
                gsl_matrix_set(transition_matrix, i, j, value);
            }
        }
}

void build_transition_matrix(FILE *fp, FILE *tr_matrix, FILE *sd, FILE *chs)
{
    gsl_matrix *transition_matrix = gsl_matrix_calloc(DIMY, DIMX);
    char **chord_set = build_chord_table(fp);

    /****************************SAVE CHORD SET****************************/
    write_chord_set(chord_set, chs);

    double *sums;
    btm_occurences(fp, transition_matrix, chord_set);
    sums = btm_sums(transition_matrix);
    btm_probability(transition_matrix, sums);

    /******************SAVE TRANSITION PROBABILITY MATRIX******************/
    gsl_matrix_fprintf(tr_matrix, transition_matrix, "%e");

    gsl_vector *stdis = calculate_stationary_distribution(transition_matrix);

    /*********************SAVE STATIONARY DISTRIBUTION*********************/
    gsl_vector_fprintf(sd, stdis, "%e");

    /**************************TIDY UP SECTION*****************************/
    gsl_vector_free(stdis);
    gsl_matrix_free(transition_matrix);
    clean(sums);
    clean_matrix((void **)chord_set, NUM_OF_CHORDS);
}

void linear_transformation(gsl_matrix *transition_matrix)
{

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        gsl_matrix_set(transition_matrix, i, i, gsl_matrix_get(transition_matrix, i, i) - 1);
}

gsl_vector *QR_calculation(gsl_matrix *transition_matrix)
{
    int M = transition_matrix->size1,
        N = transition_matrix->size2;
    gsl_vector *x = gsl_vector_alloc(N);
    gsl_vector *b = gsl_vector_alloc(M);

    for (int j = 0; j < N; j++)
        gsl_matrix_set(transition_matrix, M - 1, j, 1);

    for (int i = 0; i < M; i++)
        gsl_vector_set(b, i, 0);

    gsl_vector_set(b, M - 1, 1);

    gsl_vector *tau = gsl_vector_alloc(N);
    gsl_vector *residual = gsl_vector_alloc(M);

    gsl_linalg_QR_decomp(transition_matrix, tau);
    gsl_linalg_QR_lssolve(transition_matrix, tau, b, x, residual);

    gsl_vector_free(tau);
    gsl_vector_free(residual);
    gsl_vector_free(b);

    return x;
}

gsl_vector *calculate_stationary_distribution(gsl_matrix *transition_matrix)
{
    gsl_vector *solution;
    linear_transformation(transition_matrix);
    solution = QR_calculation(transition_matrix);

    return solution;
}

int main()
{
    FILE *fp = fopen("build/txt_src/res.txt", "r");
    FILE *tr_matrix = fopen("build/txt_src/tr_matrix.txt", "w");
    FILE *sd = fopen("build/txt_src/stationary_distribution.txt", "w");
    FILE *chs = fopen("build/txt_src/chord_set.txt", "w");

    build_transition_matrix(fp, tr_matrix, sd, chs);

    fclose(fp);
    fclose(tr_matrix);
    return 0;
}