#include "ial.h"
#include "stdio.h"

int total;
int max_value(int a, int b) { return (a > b)? a: b; }

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

void HS_Sort(S_String *s)
{
    total = strlen(s->str) - 1;

    // Rearrange the heap according to heap property (maxheap ... root > children).
    for (int i = total / 2; i >= 0; i--)
    {
        HS_Heapify(s, i);
    }

    // Extraction of the "maximal" element.
    for (int i = total; i > 0; i--)
    {
        // Move current root to the end.
        SWAP(s->str, 0, i);
        total--;
        // Rearrange the reduced heap.
        HS_Heapify(s, 0);
    }
}

int BM_BCRule(S_String *s, S_String *search)
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