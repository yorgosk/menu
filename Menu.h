/* Author: Georgios Kamaras
   Date: 4-03-2015 */
#ifndef __MENU__
#define __MENU__

#include "TypeMenuElement.h"

typedef struct InfoMenu *InfoMenuPtr; // type of pointer at the Informational Node of Menu
typedef struct MenuNode *MenuNodePtr; // type of pointer at node

/* functions for the ADT Menu actions */

/* list creation / destruction */
InfoMenuPtr MenuCreation(char * theName);
void MenuDestruction(InfoMenuPtr * linfo, int *error);

/* control actions */
int MenuEmptyCategory(InfoMenuPtr * linfo);

/* entrance (displaying) actions */
void MenuPrintContent(InfoMenuPtr * linfo, FILE *out, int *error);
void MenuPrintCategory(InfoMenuPtr * linfo, FILE *out, int *error, int indent, int html); // 'html' parameter takes as values:
											  // 1 (not html output) and 1 (html output)
											// so the function knows if html tags are needed or not
/* insertion - deletion actions */
void MenuNodeInsertion(InfoMenuPtr * linfo, int *error, TMenuElement tempMenuNode);
void MenuChangeContent(InfoMenuPtr * linfo, FILE *in, int *error);
void MenuCategoryInsertion(InfoMenuPtr * linfo, int *error);
void MenuDelete(InfoMenuPtr * linfo, int *error);
void MenuDeleteAll(InfoMenuPtr * linfo);

/* choose node */
void MenuNext(InfoMenuPtr * linfo, int *error);
void MenuPrevious(InfoMenuPtr * linfo, int *error);
void MenuMoveForward(InfoMenuPtr * linfo, int *error);
void MenuMoveBackward(InfoMenuPtr * linfo, int *error);
void MenuMoveUpward(InfoMenuPtr * linfo, int *error);
void MenuMoveDownward(InfoMenuPtr * linfo, int *error);

/* copy - paste actions */
void MenuCut(InfoMenuPtr * linfo, int *error);
void MenuPaste(InfoMenuPtr * linfo);

#endif
