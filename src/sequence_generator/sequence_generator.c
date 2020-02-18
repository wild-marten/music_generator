#include "sequence_generator.h"

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

double **initialize_transition_matrix()
{
    double **transition_matrix = calloc(NUM_OF_CHORDS, sizeof(double *));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        transition_matrix[i] = calloc(NUM_OF_CHORDS, sizeof(double));

    return transition_matrix;
}

char **initialize_chord_set()
{
    char **chord_set = calloc(NUM_OF_CHORDS, sizeof(char *));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        chord_set[i] = calloc(CHORD_LENGHT, sizeof(char));

    return chord_set;
}

void read_matrix(FILE *matrix, double **transition_matrix)
{
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        for (int j = 0; j < NUM_OF_CHORDS; j++)
            fscanf(matrix, "%le", &transition_matrix[i][j]);
}

void read_chord_set(FILE *chs, char **chord_set)
{
    for (int i = 0; i < NUM_OF_CHORDS; i++)
        fscanf(chs, "%s", chord_set[i]);
}

double *get_stat_dis(FILE *sd)
{
    double *stat_dis = calloc(NUM_OF_CHORDS, sizeof(double));

    for (int i = 0; i < NUM_OF_CHORDS; i++)
        fscanf(sd, "%le", &stat_dis[i]);

    return stat_dis;
}

void write_abc_header(FILE *abc)
{
    fprintf(abc, "X: 1\n");
    fprintf(abc, "L: 1/4\n");
    fprintf(abc, "K: C\n");
    fprintf(abc, "|:|1-3 ");
}

int chord_id(int row, int column, int **transition_matrix)
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

gsl_rng *set_rng()
{
    const gsl_rng_type *T;
    gsl_rng *rng;
    T = gsl_rng_default;
    rng = gsl_rng_alloc(T);
    gsl_rng_env_setup();

    struct timeval tv;
    gettimeofday(&tv, 0);
    unsigned long seed = tv.tv_sec + tv.tv_usec;
    gsl_rng_set(rng, seed);

    return rng;
}

char *find_note(int root_position, int sh_cmp)
{
    char *note = calloc(NOTE_LENGHT, 1);
    int off = 0;

    if (sh_cmp % 2 == 1)
        off = 1;

    int semitone_distance = interval[sh_cmp / 2 - 1] + off;
    int sem = root_position + semitone_distance;
    int sem_pos = sem % 12;

    strcat(note, semitone[sem_pos]);

    for (int i = 0; i < semitone_distance / 12; i++)
        note[strlen(note)] = '\'';

    return note;
}

char *note(char *chord, int j)
{
    char *note = calloc(CHORD_LENGHT, 1);

    if (chord[j] == '^')
        strncat(note, chord + j, 2);

    else if (chord[j] <= 'G' && chord[j] >= 'A')
        strncat(note, chord + j, 1);
    else
        return note;

    return note;
}

int position(char *note)
{
    if (note == 0)
        return -1;
    for (int i = 0; i < NUM_OF_SEMITONES; i++)
        if (strcmp(note, semitone[i]) == 0)
            return i;
    return -1;
}

int *scale_ids(char *chord)
{
    char *root = note(chord, 1);
    int base = position(root);
    int *inter = calloc(7, sizeof(int));
    int nxt = 0, k = 0, distance;
    char *nt;

    int j = 0;

    clean(root);

    //distances
    while (j < strlen(chord))
    {
        nt = note(chord, j);
        nxt = position(nt);

        distance = 0;

        if (nxt != -1)
        {
            if (nxt < base)
                distance = NUM_OF_SEMITONES - base + nxt;
            else if (nxt > base)
                distance = nxt - base;

            for (int i = 0; i < NUM_OF_INTERVALS; i++)
            {
                if (distance == interval[i])
                {
                    inter[k] = (i + 1) * 2;
                    k++;
                    break;
                }
                else if (distance < interval[i])
                {
                    inter[k] = i * 2 + 1;
                    k++;
                    break;
                }
            }
            j += strlen(nt);
        }

        else
            j++;

        nt = clean(nt);
    }
    clean(nt);

    return inter;
}

char *sscale(char *chord)
{
    char *tonic = calloc(CHORD_LENGHT, sizeof(char));
    int *ids = scale_ids(chord);
    int how_many_tones, non_zero = 0;

    for (int i = 0; i < 7; i++)
    {
        if (ids[i] != 0)
            non_zero++;
        else
            break;
    }

    for (int i = 0; i < NUM_OF_SEMITONES; i++)
        for (int j = 0; j < NUM_OF_SCALES; j++)
        {
            how_many_tones = 0;

            for (int k = 0; k < 7; k++)
                for (int z = 0; z < NUM_OF_CMP; z++)
                {
                    if (ids[k] == 0)
                        break;

                    char *n = find_note(i, scale_cmp[j][z]);

                    if (strcmp(semitone[ids[k]], n) == 0)
                    {
                        how_many_tones++;
                        clean(n);
                        break;
                    }
                    clean(n);
                }

            if (non_zero == how_many_tones)
            {
                if (semitone[i][0] == '^')
                {
                    char *base = calloc(NOTE_LENGHT, 1);
                    base[0] = semitone[i][1];
                    base[1] = '#';
                    strcat(tonic, base);
                    clean(base);
                }
                else
                    strcat(tonic, semitone[i]);

                strcat(tonic, scales[j]);
                clean(ids);
                return tonic;
            }
        }
    clean(ids);
    return tonic;
}

bool validation(int x0, int x1, double *stat_dis, double **transition_matrix)
{
    gsl_rng *rng = set_rng();

    double sigma = gsl_rng_uniform_pos(rng);

    gsl_rng_free(rng);

    if (sigma <= (stat_dis[x1] / stat_dis[x0]))
        return true;
    else
        return false;
}

void generate_sequence(FILE *matrix, FILE *abc, FILE *chs, FILE *sd)
{
    double **transition_matrix = initialize_transition_matrix();
    char **chord_set = initialize_chord_set();
    double *stat_dis = get_stat_dis(sd);
    read_matrix(matrix, transition_matrix);
    read_chord_set(chs, chord_set);

    int speed[] = {1, 2, 4};

    write_abc_header(abc);
    /* DEAFULT NUMBER GENERATOR SEEDING */
    gsl_rng *rng = set_rng();

    int x0, x1;

    /*  CHOOSING ARBITRARY STATE  */
    x0 = gsl_rng_get(rng) % NUM_OF_CHORDS;
    int sp = (int)(gsl_rng_get(rng) % 2);

    for (int i = 0; i < 5; i++)
    {
        sp = (int)(gsl_rng_get(rng) % 2);

        fprintf(abc, "%s", chord_set[x0]);
        fprintf(abc, "/%d ", speed[sp]);

        char *tonic = sscale(chord_set[x0]);

        if (strcmp(tonic, "") != 0)
            fprintf(abc, "[K: %s] ", tonic);

        x1 = abs(x0 + gsl_ran_gaussian(rng, 2)) % NUM_OF_CHORDS;
        char *tonic1 = sscale(chord_set[x1]);
        /*  CHOOSING NEXT CANDIDATE VIA GAUSSIAN DISTRIBUTION   */

        while (strcmp(tonic1, tonic) == 0)
        {
            x1 = abs(x0 + gsl_ran_gaussian(rng, 2)) % NUM_OF_CHORDS;
            clean(tonic1);
            tonic1 = sscale(chord_set[x1]);
        }

        if (validation(x0, x1, stat_dis, transition_matrix) == true)
            x0 = x1;

        clean(tonic);
        clean(tonic1);
    }
    fprintf(abc, ":|");

    gsl_rng_free(rng);
    clean(stat_dis);
    clean_matrix((void **)chord_set, NUM_OF_CHORDS);
    clean_matrix((void **)transition_matrix, NUM_OF_CHORDS);
}

int main()
{
    FILE *matrix = fopen("build/txt_src/tr_matrix.txt", "r");
    FILE *chs = fopen("build/txt_src/chord_set.txt", "r");
    FILE *sd = fopen("build/txt_src/stationary_distribution.txt", "r");
    FILE *abc = fopen("build/txt_src/song.abc", "w");

    if (matrix != NULL && chs != NULL && sd != NULL && abc != NULL)
        generate_sequence(matrix, abc, chs, sd);

    fclose(matrix);
    fclose(abc);
    fclose(chs);

    return 0;
}