#include "index.h"
#include "analizer.h"
/*TODO
    ***Implement 'chord' as a structure, such as:
    typedef struct chord
    {
        char *name;
        char *root;
        char *shorthand;
        int *components;
        char *execeptions;
        char *reverse;
    }chord;

    **fix inversion
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

int find_root_pos(char *chord)
{
     int j = 0;

    while (j < NUM_OF_SEMITONES)
    {
        if (semitone[j][0] == chord[0])
            break;
        j++;
    }

    if(chord[1] == 'b')
    {
        if(j>0)
            return j-1;
        else
            return NUM_OF_SEMITONES - 1;
    }

    else if(chord[1] == '#')
    {
        if(j<NUM_OF_SEMITONES - 2)
            return j+1;
        else 
            return 0;
    }

    else
        return j;

    return -1;
}

bool is_mirex_chord(char *str)
{
    if (str[0] < 'A' || str[0] > 'G')
        return 0;

    else if (((str[1] == 'b' || str[1] == '#') && str[2] == ':') || str[1] == ':')
        return 1;

    return 0;
}

void extract_chords(FILE *fp, FILE *result)
{
    char *str = calloc(CHORD_LENGHT, 1);
    char *chord = NULL;

    while (fscanf(fp, "%s", str) == 1)
    {
        if (is_mirex_chord(str))
        {
            chord = translate_to_abc(str);

            fwrite("[", 1, 1, result);
            fwrite(chord, 1, strlen(chord), result);
            fwrite("] ", 1, 2, result);
                
        }

        clean(str);
        chord = clean(chord);
        str = calloc(CHORD_LENGHT, 1);
    }
    fwrite(separator, 1, strlen(separator), result);

    clean(str);
    clean(chord);
}


char *find_note(int root_position, int sh_cmp)
{
    char *note = calloc(NOTE_LENGHT, 1);
    int off = 0;

    if (sh_cmp % 2 == 1)
        off = 1;

    int semitone_distance =  interval[sh_cmp / 2 - 1] + off;
    int sem = root_position + semitone_distance;
    int sem_pos = sem % 12;

    strcat(note, semitone[sem_pos]);

    for (int i = 0; i < semitone_distance / 12; i++)
        note[strlen(note)] = '\'';

    return note;
}

char *extract_shorthand(char *chord)
{
    char *tmp = calloc(CHORD_LENGHT, 1);
    int j = 0, k = 0;

    while (chord[j++] != ':')
        ;

    while (chord[j] != '(' && chord[j] != 0 && chord[j] != '/')
    {
        tmp[k] = chord[j];
        k++;
        j++;
    }

    return tmp;
}

int shorthand_position(char *chord)
{
    char *sh = extract_shorthand(chord);
    int j = 0;

    while (j < NUM_OF_SH)
    {
        if (strcmp(sh, shorthand[j]) == 0)
            break;
        j++;
    }
    clean(sh);
    return j;
}

char *shorthand_to_notes(char *chord, char  *abc_chord)
{
    int sh_pos = shorthand_position(chord);
    int j = 0;
    int root_pos = find_root_pos(chord);

    strcat(abc_chord, semitone[root_pos]);

    while (j < NUM_OF_CMP && shorthand_components[sh_pos][j] != 0)
    {
        char *tmp = find_note(root_pos, shorthand_components[sh_pos][j]);
        strcat(abc_chord, tmp);
        clean(tmp);
        j++;
    }

    return abc_chord;
}

char *exeption(char *abc, char *chord, int cmp)
{
    char *rmv_note = find_note(find_root_pos(chord), cmp);

    int j = 0;

    while (strncmp(abc + j, rmv_note, strlen(rmv_note)) != 0)
        j++;

    char *tmp = calloc(strlen(abc), 1);

    strncat(tmp, abc, j);
    strncat(tmp, abc + j + strlen(rmv_note), strlen(abc) - j - strlen(rmv_note));

    clean(rmv_note);
    clean(abc);

    return tmp;
}

char *extensions_to_notes(char *chord, char *abc_chord)
{
    int j = 0;

    while (chord[j] != '(' && chord[j] != 0)
        j++;

    if (chord[j] == 0)
        return abc_chord;

    char *temp = calloc(NOTE_LENGHT, 1);
    char *note=NULL;
    int root_pos = find_root_pos(chord);
    int off = 0;
    j++;

    while (chord[j] != ')' && chord[j] != 0)
    {
        if (chord[j] == '*')
        {
            while (chord[j] != ')' && chord[j] != ',')
            {
                if (chord[j] == 'b')
                    off = -1;
                else if (chord[j] == '#')
                    off = 1;
                else
                    temp[strlen(temp)] = chord[j];

                j++;
            }

            abc_chord = exeption(abc_chord, chord, atoi(temp) * 2 + off);
            clean(temp);

            temp = calloc(NOTE_LENGHT, 1);
            off = 0;
        }
        else if (chord[j] == 'b')
            off = -1;
        else if (chord[j] == '#')
            off = 1;

        else if (chord[j] != ',')
            temp[strlen(temp)] = chord[j];

        else
        {
            note=find_note(root_pos, atoi(temp) * 2 + off);
            strcat(abc_chord, note);
            note = clean(note);
            clean(temp);
            temp = calloc(NOTE_LENGHT, 1);
            off = 0;
        }

        j++;
    }

    if (temp != 0)
    {   
        note=find_note(root_pos, atoi(temp) * 2 + off);
        strcat(abc_chord, note);
    }

    clean(note);
    clean(temp);
    return abc_chord;
}

char *inversion(char *abc, char *chord)
{
    int root_pos = find_root_pos(chord);
    int j = 0, off = 0;

    while (chord[j] != '/' && chord[j] != 0)
        j++;

    if (chord[j] == 0)
        return abc;
    j++;

    char *tmp = calloc(NOTE_LENGHT, 1);

    int k = 0;
    while (chord[j] != 0)
    {
        if (chord[j] == 'b')
            off += -1;
        else if (chord[j] == '#')
            off += 1;
        else
        {
            tmp[k] = chord[j];
            k++;
        }
        j++;
    }

    char *to_inver = find_note(root_pos, atoi(tmp) * 2 + off);
    
    j = 0;
    int s = strlen(to_inver);

    while (j <= (int)(strlen(abc) - s) && strncmp(abc + j, to_inver, s) != 0)
        j++;

    char *inverted = calloc(CHORD_LENGHT, 1);

    strncat(inverted, abc + j, strlen(abc) - j);

    for (int i = 0; i < j; i++)
    {
        inverted[strlen(inverted)] = abc[i];

        if (abc[i] >= 'A' && abc[i] <= 'G')
            inverted[strlen(inverted)] = '\'';
    }

    clean(to_inver);
    clean(tmp);
    abc = clean(abc);

    return inverted;
}

char *translate_to_abc(char *chord)
{
    char *abc_chord = calloc(CHORD_LENGHT, 1);

    abc_chord = shorthand_to_notes(chord, abc_chord);
    abc_chord = extensions_to_notes(chord, abc_chord);
    abc_chord = inversion(abc_chord, chord);

    return abc_chord;
}

int main()
{
    FILE *result = fopen("res.txt", "w");
    
    for (int i = 1; i <= 888; i++)
    {
        char *num = calloc(10,1);

        sprintf(num, "%d", i);

        char *file = calloc(100,1);
        strcat(file, "songs/salami_chords.txt.~");
        strcat(file, num);
        strcat(file, "~");

        FILE *fp = fopen(file, "r");

        if (fp != NULL)
        {
            extract_chords(fp, result);
            fclose(fp); 
        }

        
        clean(num);
        clean(file);
    }
        
    fclose(result);
    return 0;
}