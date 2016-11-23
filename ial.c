#include "ial.h"

int readInt()
{
    int *result = MEM_malloc(sizeof(int));;
    int control;

    S_String *s = STR_Create(read_line());

    if(s->str[0] == '-' || s->str[0] == '+')
    {
        ERROR_exit(NUM_ERR);
    }
    
    control = STR_StringToInt(s, result);

    if(control == 0)
    {
        return (int)*result;
    }
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
        printf("G:%g", *result);
    }
    // Term is either integer or string literal.
    else
    {
        printf("S:%s", s->str);
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
    char tmp[n];

    for(int j = 0; j < n; j++, i++)
    {
        tmp[j] = s->str[i];
    }

    tmp[n] = '\0';
    S_String *s_result = STR_Create(tmp);

    return s_result;
}

static int total;
static int max_value(int a, int b) { return (a > b)? a: b; }

S_String *sort(S_String *s)
{
    HS_Sort(s);
    return s;
}

void HS_Sort(S_String *s)
{
    total = strlen(s->str) - 1;

    // Rearrange the heap according to heap property (maxheap ... root > children).
    for (int level = total / 2; level >= 0; level--)
    {
        HS_Heapify(s, level);
    }

    // Extraction of the "maximal" element.
    for (int last = total; last > 0; last--)
    {
        // Move current root to the end.
        SWAP(s->str, 0, last);
        total--;
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
    if (left <= total && s->str[left] > s->str[largest]) 
    {
        largest = left;
    }
    // If right child is larger than root or root's left child.
    if (right <= total && s->str[right] > s->str[largest])
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
    int charJump[255];
    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < 255; i++)
    {
        charJump[i] = -1;
    }

    // Fill the actual value of last occurrence of a character
    for (i = 0; i < search->len; i++)
    {
        charJump[(int) search->str[i]] = i; 
    }

    int shift = 0;  // s is shift of the pattern with respect to text

    while(shift <= (s->len - search->len))
    {
        int j = search->len-1;
        
        /* Keep reducing index j of pattern while characters of
           pattern and text are matching at this shift s */
        while(j >= 0 && search->str[j] == s->str[shift+j])
        {
            j--;
        }

        /* If the pattern is present at current shift, then index j
           will become -1 after the above loop */
        if (j < 0)
        {
            return shift;
            
            /* Shift the pattern so that the next character in text
               aligns with the last occurrence of it in pattern.
               The condition s+m < n is necessary for the case when
               pattern occurs at the end of text */
            shift += (shift+search->len < s->len) ? search->len - charJump[(int)s->str[shift+search->len]] : 1;
 
        }
        else
        {
            /* Shift the pattern so that the bad character in text
               aligns with the last occurrence of it in pattern. The
               max function is used to make sure that we get a positive
               shift. We may get a negative shift if the last occurrence
               of bad character in pattern is on the right side of the
               current character. */
            shift += max_value(1, j - charJump[(int)s->str[shift+j]]);
        }
    }
    return -1;
}

char *read_line()
{
    char *line = MEM_malloc(100);
    char *line_start = line;
    size_t length_max = 100;
    size_t length = length_max;
    int c;

    if(line == NULL)
    {
        return NULL;
    }

    while((c = fgetc(stdin)) != EOF)
    {    
        if(--length == 0) 
        {
            length = length_max;
            char * line_new = MEM_realloc(line_start, length_max *= 2);

            if(line_new == NULL)
            {
                MEM_free(line_start);
                return NULL;
            }

            line = line_new + (line - line_start);
            line_start = line_new;
        }

        if((*line++ = c) == '\n')
        {
            line--;
            break;
        }
    }
    *line = '\0';
    return line_start;
}

bool verify_double(S_String *s, double *result)
{
	S_String *s_after = NULL;
	S_String *s_before = NULL;

    if(!(STR_StringToDouble(s, result) == 0))
    {
    	return false;
    }

    if((s->str[0] == '-' || s->str[0] == '+'))
    {
    	return false;
    }

    if(!(STR_GetBeforeEmpty(s, &s_before ,'.') == 0))
    {
    	return false;
    }

    if(!((STR_GetAfter(s, &s_after, '.') == 0) || (strchr(s->str, 'e') != NULL) || (strchr(s->str, 'E') != NULL)))
    {
    	return false;
    }
    return true;
}