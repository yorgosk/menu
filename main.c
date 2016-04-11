/* Author: Georgios Kamaras
   Date: 4-03-2015 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>	// for isdigit(int); function
#include "Menu.h"
#include "TypeMenuElement.h"

void print_options();
int get_option();
void HTML_Input(InfoMenuPtr *CurrentMenu, char *theName);
int HTML_Read(FILE *in, TMenuElement * V, int Category);
void HTML_Output(InfoMenuPtr *CurrentMenu, char *theName);

int main(int argc, char *argv[]) {
	InfoMenuPtr FoodMenu = NULL,	// restaurant's menu
		    CafeMenu = NULL,	// cafeteria's menu
		    CurrentMenu;	// which of the above two menus we are currently working with

	TMenuElement tempMenuNode;	// for reading a newly created node's content

	int choise, option, error, indent = 0;
	char answer, restName[100], cafeName[100], buf[100];

	do {
		fprintf(stdout, "Please select the menu with which you want to start interacting. Type '1' for FoodMenu or '2' for CafeMenu:\n");
		fscanf(stdin, "%d", &choise);
		// the following 2 lines mean to fix an input bug rooting by stdin keeping (ENTER)
		// and transfering it as initial input at get_option() function
        	char ch;
       		while((ch=getchar())!='\n');    //read the rest of input

		if( choise == 1 )
        	{
        		fprintf(stdout, "You will start interacting with the FoodMenu\n");
                	CurrentMenu = FoodMenu;
        	}
        	else if ( choise == 2 )
        	{
        		fprintf(stdout, "You will start interacting with the CafeMenu\n");
                	CurrentMenu = CafeMenu;
        	}
        	else fprintf(stdout, "Wrong input! Try Again: \n");
	} while( choise != 1 && choise != 2 );

	do {
		option = get_option();
		switch (option) {
		case 0:
			fprintf(stdout, "\nExiting!\n\n");
			break;
		case 1:
			if( CurrentMenu != NULL )
			{
				fprintf(stdout, "This menu already exists. If you wish to overwrite it for a new one, press 'Y': ");
				fscanf(stdin, "%c", &answer);
				// the following 2 lines mean to fix an input bug rooting by stdin keeping (ENTER)
                                // and transfering it as initial input at get_option() function
                                char ch;
                                while((ch=getchar())!='\n');    //read the rest of input

				if( answer != 'Y') break; // we re-create a menu only if user presses 'Y'
			}

			do {
                		fprintf(stdout, "Do you want to preload a menu from an \"InputForBonus.html\" file? (y,n)\n");
                		fscanf(stdin, "%c", &answer);
				// the following 2 lines mean to fix an input bug rooting by stdin keeping (ENTER)
                		// and transfering it as initial input at get_option() function
                		char ch;
                		while((ch=getchar())!='\n');    //read the rest of input
        		} while( answer != 'y' && answer != 'n' );

        		if( answer == 'y' )
        		{
				if( choise == 1 )
				{
					HTML_Input(&CurrentMenu, restName);
					fprintf(stdout, "Restaurant is named %s\n", restName);
				}
				else
				{
					HTML_Input(&CurrentMenu, cafeName);
        				fprintf(stdout, "Cafeteria is named %s\n", cafeName);
				}
			}
        		else
        		{
                		fprintf(stdout, "Give restaurant's name: ");
                		fscanf(stdin, "%[^\n]%*c", buf);        // the specification I used basically tells, that the input is not over until we see th$
                		if( choise == 1 )
				{
					strcpy(restName, buf);
                			fprintf(stdout, "Restaurant is named %s\n", restName);
        				CurrentMenu = MenuCreation(restName);
				}
				else
				{
					strcpy(cafeName, buf);
                                        fprintf(stdout, "Restaurant is named %s\n", cafeName);
                                        CurrentMenu = MenuCreation(cafeName);
				}
			}

			if( choise == 1 )
                        {
                                fprintf(stdout, "\nFoodMenu list created!\n");
				FoodMenu = CurrentMenu;	// point FoodMenu to CurrentMenu
                        }
                        else if ( choise == 2 )
                        {
                                fprintf(stdout, "\nCafeMenu list created!\n");
                                CafeMenu = CurrentMenu;	// point CafeMenu to CurrentMenu
                        }
			break;
		case 2:
			if( CurrentMenu == NULL )	// so that we don't get a segmentation fault
			{
				fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
				break;
			}

			if( MenuEmptyCategory(&CurrentMenu) )	// check for empty category
				fprintf(stdout, "\nCurrent menu's category is empty!\n");
			else
				fprintf(stdout, "\nCurrent menu's category is not empty!\n");
			break;
		case 3:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			fprintf(stdout, "Give new menu node's content (plate/category name - 0 if plate 1 if category - (if plate) price):\n");
			error = TEReadValue(stdin, &tempMenuNode);	// read new node's content
			if( !error ) MenuNodeInsertion(&CurrentMenu, &error, tempMenuNode);	// insert a new node, with the read content
			if( error == 1 ) fprintf(stdout, "\nMemory problem, new menu node couldn't be created!\n");
			else if( error == 2 ) fprintf(stdout, "\nError! Invalid Category's code-number!\n");
			else fprintf(stdout, "\nNew menu node was successfully created!\n");
			break;
		case 4:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuPrintContent(&CurrentMenu, stdout, &error);
			if( error ) fprintf(stdout, "\nThere was a printing problem\n");
			fprintf(stdout, "\n");
			break;
		case 5:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			fprintf(stdout, "Give the renewed menu node's content (plate/category name - (if plate) price):\n");
			MenuChangeContent(&CurrentMenu, stdin, &error);	// change node's content
			if( error ) fprintf(stdout, "\nThere was an input problem!\n");
			else fprintf(stdout, "\nContent was successfully changed\n");
			break;
		case 6:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuNext(&CurrentMenu, &error);		// go next if you can
			if( error ) fprintf(stdout, "\nThere is no next node!\n");
			break;
		case 7:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuPrevious(&CurrentMenu, &error);	// go previous if you can
			if( error ) fprintf(stdout, "\nThere is no previous node!\n");
			break;
		case 8:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuMoveForward(&CurrentMenu, &error);	// change position with next if you can
			if( error ) fprintf(stdout, "\nThere was an error in the process!\n");
			break;
		case 9:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuMoveBackward(&CurrentMenu, &error);	// change position with previous if you can
			if( error ) fprintf(stdout, "\nThere was an error in the process!\n");
			break;
		case 10:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuCategoryInsertion(&CurrentMenu, &error);	// insert the head node of the new sub-category if you can
			if( error ) printf("\nError, we are currently at a head node!\n");
			break;
		case 11:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuMoveUpward(&CurrentMenu, &error);	// go up if you can
			if( error ) fprintf(stdout, "\nThere is no upper node!\n");
			break;
		case 12:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuMoveDownward(&CurrentMenu, &error);	// go down if you can
			if( error ) fprintf(stdout, "\nThere is no lower node!\n");
			break;
		case 13:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuPrintCategory(&CurrentMenu, stdout, &error, indent, 0);	// print category if you can
			if( error ) printf("\nSomewhere, something went wrong with this printing :-/\n");
			fprintf(stdout, "\n");	// change line (for beautification reasons)
			break;
		case 14:
			if( CurrentMenu == NULL )	// can't delete from a nonexistent menu
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }
			MenuDelete(&CurrentMenu, &error);	// delete next node if you can
			if( error ) fprintf(stdout, "\nError! Deleting problem!\n");
			break;
		case 15:
			if( CurrentMenu == NULL )	// can't delete from a nonexistent menu
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }
			MenuDeleteAll(&CurrentMenu);	// delete subcategory (-ies)
			break;
		case 16:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuCut(&CurrentMenu, &error);	// cut current node if you can
			if( error ) printf("\nError! That's a head node over here!\n");
			break;
		case 17:
			if( CurrentMenu == NULL )       // so that we don't get a segmentation fault
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			MenuPaste(&CurrentMenu);	// paste cutted node right next to the current
			break;
		case 18:
			if( CurrentMenu == NULL )	// can't destroy a nonexistent menu
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }

			error = 0;
			MenuDestruction(&CurrentMenu, &error);	// destroy menu if you can
			if( error )		// if there is an error in the process
			{
				fprintf(stdout, "\nDestroying error!\n");	// let the user know
				break;
			}

			if( choise == 1 )
                        {
                                fprintf(stdout, "\nFoodMenu list destroyed!\n");
                                FoodMenu = CurrentMenu;		// FoodMenu points to CurrentMenu
                        }
                        else if ( choise == 2 )
                        {
                                fprintf(stdout, "\nCafeMenu list destroyed!\n");
                                CafeMenu = CurrentMenu;		// CafeMenu points to CurrentMenu
                        }
			break;
		case 19:
			if( choise == 1 )		// change from FoodMenu to CafeMenu
			{
				CurrentMenu = CafeMenu;
				choise = 2;
				fprintf(stdout, "\nYou will now interact with the CafeMenu\n");
			}
			else					// else change from CafeMenu to FoodMenu
			{
				CurrentMenu = FoodMenu;
				choise = 1;
				fprintf(stdout, "\nYou will now interact with the FoodMenu\n");
			}
			break;
		case 20:
			if( CurrentMenu == NULL )	// can't print a non-existent menu
                        {
                                fprintf(stdout, "\nError! The CurrentMenu doesn't even exist!\n");
                                break;
                        }
			if( choise == 1 ) HTML_Output(&CurrentMenu, restName);
			else HTML_Output(&CurrentMenu, cafeName);
			break;
		}
	}
	while (option);

	return 0;
}

void print_options()	// display user's options for interacting with the menu
{
	printf("\n");
	printf(" 0. Exit\n");
	printf(" 1. Create menu list\n");
	printf(" 2. Check for empty category\n");
	printf(" 3. Insert a new menu node\n");
	printf(" 4. Print current node's content\n");
	printf(" 5. Change current node's content\n");
	printf(" 6. Go to the next menu node\n");
	printf(" 7. Go to the previous menu node\n");
	printf(" 8. Change current node and it's sub-categories with the next node and his sub-categories\n");
	printf(" 9. Change current node and it's sub-categories with the previous node and his sub-categories\n");
	printf("10. Insert a new (sub-)category\n");
        printf("11. Go to the upper category\n");
        printf("12. Go to the lowwer category\n");
        printf("13. Print current (sub-)category\n");
        printf("14. Delete next menu node\n");
        printf("15. Delete all nodes of current (sub-)category\n");
        printf("16. Cut a node\n");
        printf("17. Paste a node\n");
        printf("18. Destroy menu list\n");
	printf("19. Change the menu with which you interact (Food or Cafe)\n");
	printf("20. Print CurrentMenu at an \"OutputForBonus.html\" document\n");
	printf("Enter your input (0-20): ");
}

int get_option()	// get user's option
{
	char buf[100];	// reading buffer
	int option;

	option = -1;
	while (option < 0 || option > 20) {	// while user requests unavailable options
		print_options();
		fgets(buf, sizeof(buf), stdin);
		sscanf(buf, "%d", &option);		// read option from buf
	}
	return option;		// return option to main
}

void HTML_Input(InfoMenuPtr *CurrentMenu, char *theName)
{
	FILE *fp;

	int error, i, latest_num = 0, br = 0; //--->for the categories' headers

	char *buf, ch;
	int N, check;

	fp = fopen("InputForBonus.html", "r");		// open .html file to read

	TMenuElement tempMenuNode;

	while( (ch = fgetc(fp)) != EOF )
	{
		switch(ch)
		{
			case '<':		// keep track for unclosed brackets
				br++;
				break;
			case '>':
				br--;		// keep track for unclosed brackets
				break;
			case '/':
				do{ ch = fgetc(fp); } while(ch!='>');	// there is a closing tag
				br--;
				break;
			case '!':		// there is a comment tag (it's content it's irrelevant for us)
				do{ ch = fgetc(fp); } while(ch != '>');
				br--;
				break;
			case 'p':	// for us (new paragraph <p>) == (new plate)
				ch = fgetc(fp); // read '>'
                        	if( ch == '>' ) br--;
                        	error = HTML_Read(fp, &tempMenuNode, 0);	// read plate's content
                        	if( !error ) MenuNodeInsertion(CurrentMenu, &error, tempMenuNode);	// create (plate's) node
                        	else
				{
					fprintf(stdout, "\nThere was an input error!\n");
                        		return;
				}
				break;
			case 'b':	// <body> or </body> tag
				for( i=1; i<=3; i++ ) ch = fgetc(fp);	// read 'o', 'd', 'y'
				ch = fgetc(fp); // read '>'
                        	if( ch == '>' ) br--;
				break;
			case 'h':	// new category node
				ch = fgetc(fp);
				check = ch-'0';
				if( !isdigit(ch) )  // it's 'head'
				{
					do{ ch = fgetc(fp); }while( ch != '>' );
                        	       	br--;   // has read '>'
				}
				else
				{
					if( check <= latest_num )
					{
						latest_num++;
						do {
                                                	MenuMoveUpward(CurrentMenu, &error);	// go up until we reach the category where we are going to insert the new node
                                                	if( error )
                                                	{
                                                        	fprintf(stdout, "\nThere is no upper node!\n");
                                                        	return;
                                                	}
	                                        } while( check < --latest_num );
						latest_num = check;
						//new category node (next)
						ch = fgetc(fp); // read '>'
        		            		if( ch == '>' ) br--;

						error = HTML_Read(fp, &tempMenuNode, 1);		// read new node's content
                       				if( !error ) MenuNodeInsertion(CurrentMenu, &error, tempMenuNode); // insert new node
                       				else
                       				{
                               				fprintf(stdout, "\nThere was an input error!\n");
                               				return;
                       				}

						MenuCategoryInsertion(CurrentMenu, &error);	// insert subcategory
        	                                if( error )
                	                        {
                        	                        printf("\nError, we are currently at a head node!\n");
                                	                return;
                                        	}

	                                        MenuMoveDownward(CurrentMenu, &error);	// go to the subcategory's head node
        	                                if( error )
                	                        {
                        	                        fprintf(stdout, "\nThere is no lower node!\n");
                                	                return;
                                        	}
					}
					else
					{
						latest_num = check;
						//new sub-category node (down)
						ch = fgetc(fp); // read '>'
                                        	if( ch == '>' ) br--;

						error = HTML_Read(fp, &tempMenuNode, 1);	// read new node's content
                                        	if( !error ) MenuNodeInsertion(CurrentMenu, &error, tempMenuNode);	// insert new node
                                        	else
                                        	{
	                                	        fprintf(stdout, "\nThere was an input error!\n");
                                        	        return;
                                        	}

						MenuCategoryInsertion(CurrentMenu, &error);	// insert new subcategory
                                        	if( error )
                                        	{
                                                	printf("\nError, we are currently at a head node!\n");
                                                	return;
                                        	}

                                        	MenuMoveDownward(CurrentMenu, &error);	// go down
                                        	if( error )
                                        	{
                                        	        fprintf(stdout, "\nThere is no lower node!\n");
                                               		return;
                                        	}
					}
				}
				break;
			case 't':	// read .html file's title which includes the restaurant's or (cafe's) name
				ch = fgetc(fp);
                	        do{ ch = fgetc(fp); }while( ch != '>' );
                	        br--;   // has read '>'

	       			i = 0;
				N = 2;
       				buf = malloc(N * sizeof(char));
       				if( buf == NULL )
       				{
               				fprintf(stdout, "\nMalloc problem HTML_Input()\n");
               				return;
       				}

				while( (ch = fgetc(fp)) != '<' )
        		    	{		// read restaurant's or (cafe's) name
                       			if( i<N ) buf[i++] = ch;
                       			else
                       			{
                               			N += N;
                               			buf = realloc(buf, N * sizeof(char));
                               			if( buf == NULL )
                               			{
                                       			fprintf(stdout, "\nRealloc problem HTML_Input()\n");
                                       			return;
                               			}
                               			else buf[i++] = ch;
                       			}
				}
				buf[i] = '\0';		// string's last character is the NULL character
				ungetc(ch, fp);		// unget the character that trigered the end of the reading
				theName = malloc(100 * sizeof(char));
				strcpy(theName, buf);		// copy name string from reading buffer
				*CurrentMenu = MenuCreation(theName);	// create menu
				break;
		}
	}

	free(buf);      // free dynamically allocated memory for reading buffer

	fclose(fp);	// close reading file
}

int HTML_Read(FILE *in, TMenuElement * V, int Category)
{
	char *buf, ch;
	int returned = 1, i = 0, N = 2, whiChar = 0;
	buf = malloc(N * sizeof(char));
	if( buf == NULL )
	{
		fprintf(stdout, "\nMalloc problem HTML_Read()\n");
		return 1;
	}

	if( !Category )		// plate
	{
		while( !isdigit(ch = fgetc(in)) && whiChar < 2 )	//read until we reach the price (serial number after plate is allowed)
                {		// read name string
                        if( i<N ) buf[i++] = ch;
                        else
                        {
                                N += N;
                                buf = realloc(buf, N * sizeof(char));
                                if( buf == NULL )
                                {
                                        fprintf(stdout, "\nRealloc problem HTML_Read()\n");
                                        return 1;
                                }
                                else buf[i++] = ch;
                        }
                        if( isspace(ch) ) whiChar++;	// white character
                }
	}
	else		// category
	{
		while( (ch = fgetc(in)) != '<' )	// read until we reach the start of the </hx> tag
                {		// read name string
                        if( i<N ) buf[i++] = ch;
                        else
                        {
                                N += N;
                                buf = realloc(buf, N * sizeof(char));
                                if( buf == NULL )
                                {
                                        fprintf(stdout, "\nRealloc problem\n");
                                        return 1;
                                }
                                else buf[i++] = ch;
                        }
                }
	}

	buf[i] = '\0';		// string's last character is the NULL character
	ungetc(ch, in);		// unget the character that trigered the end of the reading
	V->name = malloc((i+1) * sizeof(char));
	strncpy(V->name, buf, i);	// copy name string to it's position inside the menu node's 'Data'
	V->Category = Category;		// save if category or not
	if( !Category ) returned = fscanf(in, "%f", &(V->Price));	// if plate (not category), read price
	else V->Price = 0;		//if category, for typical reasons, set price as 0

	free(buf);	// free dynamically allocated memory for reading buffer

	return !returned;
}

void HTML_Output(InfoMenuPtr *CurrentMenu, char *theName)
{
	FILE *fp;

	int error;

	fp = fopen("OutputForBonus.html", "w");		// open or create (if it doesn't exists) file to write

	fprintf(fp, "<!DOCTYPE html>\n");		// print initial html tags
	fprintf(fp, "<html>\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<title>%s</title>\n", theName);
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body>\n");

	error = 0;
	while( !error ) MenuMoveUpward(CurrentMenu, &error);	// go to the <h1> level
	error = 0;
        while( !error ) MenuPrevious(CurrentMenu, &error);	// go to the menu's head node
	error = 0;
	do {
		MenuNext(CurrentMenu, &error);			// go next
		if( !error )MenuPrintCategory(CurrentMenu, fp, &error, 1, 1);	// print categories with html tags
	} while( !error );			// while we can go next

	fprintf(fp, "\n</body>\n");		// html closing tags
	fprintf(fp, "</html>");

	fclose(fp);		// close writing file
}
