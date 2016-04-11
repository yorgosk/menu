/* Author: Georgios Kamaras
   Date: 4-03-2015 */
#include "TypeMenuElement.h"

void TESetValue(TMenuElement *Lhs, TMenuElement Rhs) // Lhs = Rhs
{
	*Lhs = Rhs;	// setting the value
}

int TEPrintValue(FILE *out, TMenuElement V)
{
	int result;	// here we will store what we are going to return for error diagnose

	if( V.Category )
		result = fprintf(out, "It's the %s category of plates.", V.name);	// printing for category of plates
	else
		result = fprintf(out, "It's the %s plate, with a price of %4.2f $.", V.name, V.Price);	// printing for plate

	if(result > 0) return 0;	// no error
	else return 1;			// some error
}

int TEReadValue(FILE *in, TMenuElement * V)
{
	int result;	// here we will store what we are going to return for error diagnose
	char buf[100];	// a buffer for reading

	result = fscanf(in, "%[^\n]%*c", buf);	// the specification I used basically tells, that the input is not over until we see the 'new line' character ('\n')
	V->name = malloc(100 * sizeof(char));	// dynalically allocate memory for the name
	strcpy(V->name, buf);		// copy sting of buffer to the string of plate's/category's name

	result = fscanf(in, "%d", &(V->Category));	// read if category (1) or not (1)
	if( V->Category == 0 ) result = fscanf(in, "%f", &(V->Price));	// if not category (so, a plate) read price
	else if( V->Category == 1)		// if category...
	{
		V->Price = 0;		// store 0 as price for typical reasons
	}
	else
	{
		// the following 2 lines mean to fix an input bug rooting by stdin keeping (ENTER)
        	// and transfering it as initial input at get_option() function
        	char ch;
        	while((ch=getchar())!='\n');    //read the rest of input

		return 2;			// for invalid Category code-number, so that we can diagnose the problem from main
	}

	// the following 2 lines mean to fix an input bug rooting by stdin keeping (ENTER)
        // and transfering it as initial input at get_option() function
	char ch;
        while((ch=getchar())!='\n');    //read the rest of input

	return !result;		// return error diagnose's value
}

int TEChangeValue(FILE *in, TMenuElement * V)
{
	int result;	// here we will store what we are going to return for error diagnose
	char buf[100];	// a buffer for reading

	result = fscanf(in, "%[^\n]%*c", buf);	// the specification I used basically tells, that the input is not over until we see the 'new line' character ('\n')
	V->name = malloc(100 * sizeof(char));	// dynalically allocate memory for the name
	strcpy(V->name, buf);		// copy sting of buffer to the string of plate's/category's name

	if( V->Category == 0 )
	{
		result = fscanf(in, "%f", &(V->Price));  // if not category (so, a plate) read price
        	// the following 2 lines mean to fix an input bug rooting by stdin keeping (ENTER)
        	// and transfering it as initial input at get_option() function
        	char ch;
        	while((ch=getchar())!='\n');    //read the rest of input
	}
	else           // if category...
        {
                V->Price = 0;           // store 0 as price for typical reasons
        }

        return !result;         // return error diagnose's value
}

int TECategory(TMenuElement V)
{
	return V.Category;	// return if category (1) or not (0)
}
