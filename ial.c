/* --- Libraries list --- */
#include "ial.h"

/* --- Static global variables --- */
static int hs_total;

/* --- Function definition --- */
int readInt()
{
    int *result = MEM_malloc(sizeof(int));

    // Creates a string from standard input.
    S_String *s = STR_Create(read_line());
    
    // Term meets requirements for integer literal.
    if(verify_integer(s, result) == true)
    {
        return *result;
    }
    // Term doesn't meet requirements for integer literal.
    else
    {
        ERROR_exit(NUM_ERR);
    }
    return 0;
}

double readDouble()
{
    double *result = MEM_malloc(sizeof(int));;

    // Creates a string from standard input.
    S_String *s = STR_Create(read_line());

    // Term meets requirements for double literal.
    if(verify_double(s, result) == true)
    {
        return *result;
    }
    // Term doesn't meet requirements for double literal.
    else
    {
    	ERROR_exit(NUM_ERR);
    }
    return 0.0;
}

S_String *readString()
{
    S_String *s = STR_Create(read_line());

    return s;
}

void print(S_String *s)
{
    double *result = MEM_malloc(sizeof(double));

    // Term meets requirements for double literal.
    if(verify_double(s, result) == true)
    {
        printf("%g", *result);
    }
    // Term is either integer or string literal.
    else
    {
        printf("%s", s->str);
    }
    MEM_free(result);
}

int length(S_String *s)
{
    return strlen(s->str);
}

int compare(S_String *s1, S_String *s2)
{
    return strcmp(s1->str, s2->str);
}

S_String *substr(S_String *s, int i, int n)
{
    // Invalid arguments.
    if(i < 0 || n < 0)
    {
    	ERROR_exit(RANDOM_ERR);
    }
    // Starting index is out of bonds or pointing at null character.
    else if(i >= s->len)
    {
    	ERROR_exit(RANDOM_ERR);
    }
    // The resulting string will exceed the null character.
    else if(i + n > s->len)
    {
    	ERROR_exit(RANDOM_ERR);
    }
    // Inicialization of a temporary array.
    char tmp[n];
    // Cycle copies values from the string array to the temporary array.
    for(int j = 0; j < n; j++, i++)
    {
        tmp[j] = s->str[i];
    }
    // Insert null character to the end of the temporary array.
    tmp[n] = '\0';
    // Create the resulting string string from the temporary array.
    S_String *s_result = STR_Create(tmp);
    return s_result;
}

S_String *sort(S_String *s)
{
    HS_Sort(s);
    return s;
}

void HS_Sort(S_String *s)
{
    hs_total = strlen(s->str) - 1;

    // Rearrange the heap according to heap property (maxheap ... root > children).
    for (int level = hs_total / 2; level >= 0; level--)
    {
        HS_Heapify(s, level);
    }

    // Extraction of the "maximal" element.
    for (int last = hs_total; last > 0; last--)
    {
        // Move current root to the end.
        SWAP(s->str, 0, last);
        hs_total--;
        // Rearrange the reduced heap.
        HS_Heapify(s, 0);
    }
}

void HS_Heapify(S_String *s, int i)
{
    int largest = i;        // Root is the largest.
    int left = 2 * i;       // Left child.
    int right = 2 * i + 1;  // Right child.

    // If left child is larger than root.
    if (left <= hs_total && s->str[left] > s->str[largest]) 
    {
        largest = left;
    }
    // If right child is larger than root or root's left child.
    if (right <= hs_total && s->str[right] > s->str[largest])
    {
        largest = right;
    }
    // If root is not the largest element anymore then make the swap.
    if (largest != i)
    {
        SWAP(s->str, i, largest);
        HS_Heapify(s, largest);
    }
}

int find(S_String *s, S_String *search)
{
    return BM_BCR(s, search);
}

int BM_BCR(S_String *s, S_String *search)
{
    // An array that stores the index of last occurence of specific character from given alphabet.
    int charJump[256];
    // Shift of the pattern with respect to text
    int shift = 0;

    // Initialize all occurrences to default unwanted value of -1.
    for (int i = 0; i < 256; i++)
    {
        charJump[i] = -1;
    }

    // Fill the index of last occurrence of a specific character in the pattern.
    for (int j = 0; j < search->len; j++)
    {
        charJump[(int) search->str[j]] = j; 
    }

    // Cycle while there is still a possibility of shifting the pattern.
    while(shift <= (s->len - search->len))
    {
        int k = search->len - 1;
        // Keep reducing index k while characters of pattern and text at the current shift are matching.
        while(k >= 0 && search->str[k] == s->str[shift+k])
        {
            k--;
        }

        // If the pattern is present at the current shift, then value of index k is equal to -1.
        if (k < 0)
        {
            return shift;
        }
        else
        {
            /* Shift the pattern so that the bad character in text aligns with the last occurrence of "itself" in the pattern.
            If the bad character is not present in the pattern, then increase shift by the length of the pattern.
            The "greater value" is used to make sure that we get a positive shift. 
            We may get a negative shift if the last occurrence of bad character in pattern is on the right side of the current character. */
            int max_value = (1 > k - charJump[(int)s->str[shift+k]]) ? 1 : k - charJump[(int)s->str[shift+k]];
            shift += max_value;
        }
    }
    // If text does not contain a pattern then return value -1.
    return -1;
}

bool verify_integer(S_String *s, int *result)
{
    // Term can't be converted to a value of integer literal.
    if(!(STR_StringToInt(s, result) == 0))
    {
        return false;
    }
    // Term contains unwanted sign characters.
    if((s->str[0] == '-' || s->str[0] == '+'))
    {
        return false;
    }
    // If term doesn't break any of the above rules it can be verified as a valid integer literal.
    return true;
}

bool verify_double(S_String *s, double *result)
{
	S_String *s_after = NULL;
	S_String *s_before = NULL;

    // Term can't be converted to a value of double literal.
    if(!(STR_StringToDouble(s, result) == 0))
    {
        return false;
    }
    // Term contains unwanted sign characters.
    if((s->str[0] == '-' || s->str[0] == '+'))
    {
    	return false;
    }
    // Term does not contain an integer part before the dot character if term is in dot notation.
    if((STR_FindChar(s, '.') != -1) && !(STR_GetBeforeEmpty(s, &s_before ,'.') == 0))
    {
        return false;
    }
    // Term has to be either in the dot notation or the exponential (scientific) notation.
    if(!((STR_GetAfter(s, &s_after, '.') == 0) || (strchr(s->str, 'e') != NULL) || (strchr(s->str, 'E') != NULL)))
    {
    	return false;
    }
    // If term doesn't break any of the above rules it can be verified as a valid double literal.
    return true;
}

char *read_line()
{
    char *line = MEM_malloc(100);
    char *line_start = line;
    size_t length_max = 100;
    size_t length = length_max;
    int c;

    // Unsuccessful allocation of memory.
    if(line == NULL)
    {
        return NULL;
    }
    // Main read cycle for stdin.
    while((c = fgetc(stdin)) != EOF)
    {    
        // When the whole allocated memory is filled.
        if(--length == 0) 
        {
            // Increase the length back to maximum.
            length = length_max;
            // Make a reallocation of memory for doubled length (size).
            char * line_new = MEM_realloc(line_start, length_max *= 2);

            // Unsuccessful reallocation of memory.
            if(line_new == NULL)
            {
                // Free the first chunk of allocated memory.
                MEM_free(line_start);
                return NULL;
            }
            // Pointer to the current position of saved line.
            line = line_new + (line - line_start);
            // Pointer to the starting position of saved line.
            line_start = line_new;
        }
        // Following character is '\n'.
        if((*line++ = c) == '\n')
        {
            // Decrease the pointer value (move one character back).
            line--;
            // Force the end of the main read cycle.
            break;
        }
    }
    // Insert a null character.
    *line = '\0';
    // Return the read line from stdin.
    return line_start;
}