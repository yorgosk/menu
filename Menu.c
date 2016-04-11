/* Author: Georgios Kamaras
   Date: 4-03-2015 */
#include <stdlib.h>
#include "Menu.h"
#include "TypeMenuElement.h"

typedef struct InfoMenu {	//the Informational Node
	char* 	  Name; 	   // name of the restaurant
	MenuNodePtr Start;	   // pointer to the first node of the upper category
	MenuNodePtr Current;   // pointer to the current node
	MenuNodePtr CutToPaste;// pointer to the node to be transfered
} InfoMenu;

typedef struct MenuNode {	// node for category or plate
	MenuNodePtr Prev, Next, Up, Down; // pointers for the double-linked list
	TMenuElement Data; // pointers for category-subcategory
} MenuNode;

/* functions for the ADT Menu actions */

InfoMenuPtr MenuCreation(char * theName)
{
	InfoMenuPtr linfo;			// info node
	linfo = malloc(sizeof(InfoMenu));	// dynamically allocate memory for info node
	MenuNodePtr head;			// head node
	head = malloc(sizeof(MenuNode));	// dynamically allocate memory for head node
	head->Up = NULL;			// head is currently pointing to nowhere
	head->Down = NULL;
	head->Prev = NULL;
	head->Next = NULL;
	linfo->Name = theName;			// restaurant's / cafeteria's name at info node
	linfo->Start = head;			// menu's list starts at head
	linfo->Current = head;			// we want to currently be at head
	linfo->CutToPaste = NULL;		// no node is Cut To be Pasted
	return linfo;				// return the info node that was just created
}

int MenuEmptyCategory(InfoMenuPtr * linfo)
{
	if( (*linfo)->Current->Prev == NULL && (*linfo)->Current->Next == NULL ) return 1;	// empty
	else return 0;	// not empty
}

void MenuNodeInsertion(InfoMenuPtr * linfo, int *error, TMenuElement tempMenuNode)
{
	MenuNodePtr newMenuNode;			// menu node to be inserted
	newMenuNode = malloc(sizeof(MenuNode));		// dynamically allocate memory for the menu node to be inserted
	if( newMenuNode == NULL )		// memory allocation problem
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;		// no error
	TESetValue(&(newMenuNode->Data), tempMenuNode);		// set new menu node's value
	if( (*linfo)->Current->Next != NULL ) (*linfo)->Current->Next->Prev = newMenuNode; // if there will be another node after the new, make new node it's previous
	newMenuNode->Next = (*linfo)->Current->Next;   // new node's next node
	(*linfo)->Current->Next = newMenuNode;	// new node is places right after (next) the current
	newMenuNode->Prev = (*linfo)->Current;
	newMenuNode->Down = NULL;	// no sub-category for the new node, for now
	newMenuNode->Up = (*linfo)->Current->Up;	// new node points up to whatever the current node points up
	(*linfo)->Current = newMenuNode;	// current node will be the new node
}

void MenuPrintContent(InfoMenuPtr * linfo, FILE *out, int *error)
{
	if( (*linfo)->Current->Prev == NULL )	// nothing to print at a head node
	{
		*error = 1;		// so that we can report the error (from the main function)
		return;
	}
	*error = TEPrintValue(out, (*linfo)->Current->Data);	// print value by keeping track of the 'error' status
}

void MenuChangeContent(InfoMenuPtr * linfo, FILE *in, int *error)
{
	if( (*linfo)->Current->Prev == NULL )	// nothing to change at a head node's 'Data'
        {
		*error = 1;		// so that we can report the error (from the main function)
                return;
        }
	*error = TEChangeValue(in, &((*linfo)->Current->Data));	// refresh node's value
}

void MenuNext(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Next == NULL )	// nowhere to go next
	{
		*error = 1;		// so that we can report the error (from the main function)
		return;
	}
	else
	{
		*error = 0;	// no error
		(*linfo)->Current = (*linfo)->Current->Next;	// go to the next node
	}
}

void MenuPrevious(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Prev == NULL )	// nowhere to go previous
	{
		*error = 1;		// so that we can report the error (from the main function)
		return;
	}
        else
	{
		*error = 0;	// no error
		(*linfo)->Current = (*linfo)->Current->Prev;	// go to the previous node
	}
}

void MenuMoveForward(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Next == NULL || (*linfo)->Current->Prev == NULL )	// no next node or currently head node
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;	// no error
	MenuNodePtr temp1 = (*linfo)->Current,		// current node
		    temp2 = (*linfo)->Current->Next->Next;	// next of the next node
	(*linfo)->Current->Prev->Next = (*linfo)->Current->Next; // currently previous next will be current next
	if( (*linfo)->Current->Next->Next != NULL ) (*linfo)->Current->Next->Next->Prev = (*linfo)->Current;	// if next node has a next node, inform it that it's new previous will be the current
	(*linfo)->Current->Next->Prev = (*linfo)->Current->Prev; // currently next node will have as previous the currently preious
	(*linfo)->Current->Prev = (*linfo)->Current->Next;	// current's prev will be current's next
	(*linfo)->Current->Next->Next = (*linfo)->Current;	// currently next node will have current as next
	(*linfo)->Current->Next = temp2; 	// new current's next node
	(*linfo)->Current = temp1;	// Current stays the same
}

void MenuMoveBackward(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Prev == NULL || (*linfo)->Current->Prev->Prev == NULL )	// no previous node or previous node is a head node
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;	// no error
	MenuNodePtr temp1 = (*linfo)->Current,		// current node
		    temp2 = (*linfo)->Current->Prev->Prev;	// previous of the previous node
	if( (*linfo)->Current->Next != NULL ) (*linfo)->Current->Next->Prev = (*linfo)->Current->Prev;	// if current node has a next node, inform it that it's new previous will be the current's previous
        (*linfo)->Current->Prev->Prev->Next = (*linfo)->Current;	// previous's previous node will have as next the current node
        (*linfo)->Current->Prev->Next = (*linfo)->Current->Next;	// current's previous node will have as next current's next node
        (*linfo)->Current->Next = (*linfo)->Current->Prev;	// current's next will be current's prev
        (*linfo)->Current->Prev->Prev = (*linfo)->Current;	// currently previous node will have current as previous
        (*linfo)->Current->Prev = temp2;	// new current's previous node
        (*linfo)->Current = temp1;	// Current stays the same
}

void MenuCategoryInsertion(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Prev == NULL )	// nothing to insert as head node's suub-category
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	MenuNodePtr head;	// sub-category's head node
        head = malloc(sizeof(MenuNode));	// dynamically allocate memory for sub-category's head node
	if( head == NULL )		// if memory allocation problem
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;	// no error
	(*linfo)->Current->Down = head;		// head down of the current node
	head->Up = (*linfo)->Current;		// head of sub-category points up to the current node...
	head->Down = NULL;			//...and nowhere else
	head->Prev = NULL;
	head->Next = NULL;
}

void MenuMoveUpward(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Up == NULL)	// no upper node
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}
        else
	{
		*error = 0;	// no error
		(*linfo)->Current = (*linfo)->Current->Up;	// go up
	}
}

void MenuMoveDownward(InfoMenuPtr * linfo, int *error)
{
        if( (*linfo)->Current->Down == NULL )	// no lower node
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;	// no error
	(*linfo)->Current = (*linfo)->Current->Down;	// go to sub-category's head node
}

void MenuPrintCategory(InfoMenuPtr * linfo, FILE *out, int *error, int indent, int html)
{
	MenuNodePtr init = (*linfo)->Current;	// we start printing the category from the current node
	if( html )	// if html printing, print html tags
	{
		fprintf(out, "\t");       // work for the indentation
		fprintf(out, "<h%d>", indent);
	}
	MenuPrintContent(linfo, out, error);
	if( html ) fprintf(out, "</h%d>\n", indent);	// if html printing, print html tags

	if( (*linfo)->Current->Down != NULL )
	{
		int i, ind = indent+1;
		(*linfo)->Current = (*linfo)->Current->Down;
		while( (*linfo)->Current != init )
		{
			if( (*linfo)->Current->Prev != NULL )
			{
				if( !html ) fprintf(out, "\n");
				for(i=1; i<=ind; i++) fprintf(out, "\t");	// if not at head node, work for the indentation
				if( html )	// if html printing, print html tags
				{
					if( TECategory((*linfo)->Current->Data) ) fprintf(out, "<h%d>", ind);	// tag if category
					else fprintf(out, "<p>");	// tag if plate
				}
				MenuPrintContent(linfo, out, error);
				if( html )	// if html printing, print html tags
                                {
                                        if( TECategory((*linfo)->Current->Data) ) fprintf(out, "</h%d>\n", ind); // tag if category
                                        else fprintf(out, "</p>\n");	// tag if plate
                                }
			}
			//MenuPrintContent(linfo, out, error);
			if( (*linfo)->Current->Down != NULL )	// if we can go down...
			{
				(*linfo)->Current = (*linfo)->Current->Down;	//...we go down
				ind++;	// increase identation (tabs) counter
			}
			else if( (*linfo)->Current->Next != NULL ) // else if we can go next...
				(*linfo)->Current = (*linfo)->Current->Next;	//... we go next
			else if( (*linfo)->Current->Down == NULL && (*linfo)->Current->Next == NULL )	// else if we can't go up or next
			{
				do {
					(*linfo)->Current = (*linfo)->Current->Up;	// we go up
					ind--;	// we decrease identation (tabs) counter
				} while( (*linfo)->Current->Next == NULL && (*linfo)->Current != init );	// until we are at a node which is not the initial and from which we can go next
				if( (*linfo)->Current != init ) (*linfo)->Current = (*linfo)->Current->Next;	// if we are not at the initial, we go next
			}
		}
	}
}

void MenuDelete(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Next == NULL || (*linfo)->Current->Next->Down != NULL )	// if there is no next node or there is a sub-category
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;	// no error
	MenuNodePtr temp;	// keep next node's memory position
	temp = (*linfo)->Current->Next;
	if( (*linfo)->Current->Next->Next != NULL ) (*linfo)->Current->Next->Next->Prev = (*linfo)->Current;	// if next node has a next node, it's new previous is the current node
	(*linfo)->Current->Next = (*linfo)->Current->Next->Next;	// current node's next is currently next node's next
	free(temp);	// "delete"
}

void MenuDeleteAll(InfoMenuPtr * linfo)
{
	MenuNodePtr init = (*linfo)->Current;	// we start deleting subcategory (-ies) from current node
	while( init->Down != NULL )	// until there is no sub-category
	{
		if( (*linfo)->Current->Down != NULL ) (*linfo)->Current = (*linfo)->Current->Down;	// if we can go down, we go down
		else if( (*linfo)->Current->Next != NULL ) (*linfo)->Current = (*linfo)->Current->Next;	// else if we can go next, we go next
		else
		{
			MenuNodePtr temp = (*linfo)->Current;	// keep current node's memory position
			if( (*linfo)->Current->Prev != NULL )	// if not at head node
			{
				(*linfo)->Current = (*linfo)->Current->Prev;	// go to the previous node
				(*linfo)->Current->Next = NULL;			// inform it that it's next will be NULL
			}
			else
			{
				(*linfo)->Current = (*linfo)->Current->Up;	// go to upper node
				(*linfo)->Current->Down = NULL;			// inform it that it's lower (subcategory) will be NULL
			}
			free(temp);	// "delete"
		}
	}
}

void MenuCut(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current->Prev == NULL )	// can't cut a head node
	{
		*error = 1;	// so that we can report the error (from the main function)
		return;
	}

	*error = 0;	// no error
	if( (*linfo)->Current->Next != NULL ) (*linfo)->Current->Next->Prev = (*linfo)->Current->Prev;	// if current node has next, we change it's information about it's previous node
	(*linfo)->CutToPaste = (*linfo)->Current;	// "cut" node
	(*linfo)->Current->Prev->Next = (*linfo)->Current->Next;	// currently previous next will be current's next
	(*linfo)->Current = (*linfo)->Current->Prev;	// go to the previous node
}

void MenuPaste(InfoMenuPtr * linfo)
{
	if( (*linfo)->Current->Next != NULL ) (*linfo)->Current->Next->Prev = (*linfo)->CutToPaste;	// if there is a next node, inform it about it's new previous
	(*linfo)->CutToPaste->Next = (*linfo)->Current->Next;	// pasted node's new next node
	(*linfo)->CutToPaste->Prev = (*linfo)->Current;		// pasted node's new previous node
	(*linfo)->CutToPaste->Up = (*linfo)->Current->Up;	// pasted node's upper is now current node's upper
	(*linfo)->Current->Next = (*linfo)->CutToPaste;		// "paste" node after curremt
	(*linfo)->Current = (*linfo)->CutToPaste;		// new current is the just pasted node
	(*linfo)->CutToPaste = NULL;
}

void MenuDestruction(InfoMenuPtr * linfo, int *error)
{
	if( (*linfo)->Current != (*linfo)->Start ) (*linfo)->Current = (*linfo)->Start->Next;	// if we are not in the head, go to it
	while( (*linfo)->Current != (*linfo)->Start || (*linfo)->Current->Next != NULL ) 	// both must be false to stop
	{						// so we are iterating until we reach the head having destroyed the rest of the menu
		if( (*linfo)->Current->Down != NULL ) MenuDeleteAll(linfo);			// if there are subcategories, delete them
		else if( (*linfo)->Current->Next != NULL )					// else if there is next
		{
			if( (*linfo)->Current->Next->Down == NULL ) MenuDelete(linfo, error);	// delete him
			else (*linfo)->Current = (*linfo)->Current->Next;			// go to the next
		}
		else (*linfo)->Current = (*linfo)->Current->Prev;	// if no next and no subvategories go to the previous node, so we can delete current node
	}
	free( (*linfo)->Current );	// delete FIRST head-node (start-node)
	free( *linfo ); // possible stack-runtime-error for double deletion !!!
	*linfo = NULL;	// current menu is NULL
}
