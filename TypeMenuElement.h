/* Author: Georgios Kamaras
   Date: 4-03-2015 */
#ifndef __TSMENU__
#define __TSMENU__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// for malloc

typedef struct Node {
        char * name;    // Name of Category or Plate
        int Category;   // 0 if Plate, 1 if Category
        float Price;    // Valid only if Plate
} TMenuElement;

void TESetValue(TMenuElement *Lhs, TMenuElement Rhs); // Lhs = Rhs
int TEPrintValue(FILE *out, TMenuElement V);	// printing menu node's value
int TEReadValue(FILE *in, TMenuElement * V);	// reading menu node's value
int TEChangeValue(FILE *in, TMenuElement * V);    // changing menu node's value
int TECategory(TMenuElement V);	// my addition, returns 1 if it is a category and 0 if it is not

#endif
