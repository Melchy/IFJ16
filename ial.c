#include "ial.h"

int total;

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