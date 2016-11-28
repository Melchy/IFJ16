/* 
* Implementace interpretu imperativniho jazyka IFJ16
* Tym 028
* xopich00 | Tomas Opichal
* xkolar64 | Michal Kolar
* xstehl17 | Vaclav Stehlik
* xleont01 | Leonteva Valeriia
* xmotyc06 | Michal Motycka
*/

#include "error.h"
#include "mem.h"
#include "fio.h"

#include <stdio.h>
#include <stdlib.h>

static char LEX_MESSAGE[] = "Lexikalni chyba";
static char SYN_MESSAGE[] = "Syntakticka chyba";
static char SEM_DEF_MESSAGE[] = "Semanticka chyba - nedefinovana promenna, funkce, nebo redefinice";
static char SEM_TYPE_MESSAGE[] = "Semanticka chyba - konflikt datovych typu, nepovolena operace";
static char SEM_OTHER_MESSAGE[] = "Semanticka chyba - jina";
static char NUM_MESSAGE[] = "Behova chyba pri nacitani ciselne hodnoty ze vstupu";
static char INIT_MESSAGE[] = "Behova chyba pri praci s neinicializovanou promennou";
static char DIV_NULL_MESSAGE[] = "Behova chyba pri deleni nulou";
static char RANDOM_MESSAGE[] = "Jina chyba";
static char ENV_MESSAGE[] = "Vnitrni chyba programu - neexistujici soubor, nedostatek pameti ...";

void ERROR_exit(int errCode)
{
	MEM_clearAll();
	FIO_Close();
	switch(errCode){
		case LEX_ERR:
			fprintf(stderr, "CHYBA: %s\n", LEX_MESSAGE);
			break;
		case SYN_ERR:
			fprintf(stderr, "CHYBA: %s\n", SYN_MESSAGE);
			break;
		case SEM_ERR_DEF:
			fprintf(stderr, "CHYBA: %s\n", SEM_DEF_MESSAGE);
			break;
		case SEM_ERR_TYPE:
			fprintf(stderr, "CHYBA: %s\n", SEM_TYPE_MESSAGE);
			break;
		case SEM_ERR_OTHER:
			fprintf(stderr, "CHYBA: %s\n", SEM_OTHER_MESSAGE);
			break;
		case NUM_ERR:
			fprintf(stderr, "CHYBA: %s\n", NUM_MESSAGE);
			break;
		case INIT_ERR:
			fprintf(stderr, "CHYBA: %s\n", INIT_MESSAGE);
			break;
		case DIV_NULL_ERR:
			fprintf(stderr, "CHYBA: %s\n", DIV_NULL_MESSAGE);
			break;
		case RANDOM_ERR:
			fprintf(stderr, "CHYBA: %s\n", RANDOM_MESSAGE);
			break;
		case ENV_ERR:
			fprintf(stderr, "CHYBA: %s\n", ENV_MESSAGE);
			break;
	}
	exit(errCode);
}

void ERROR_exit2(int errCode, void *ptrtofree)
{
	free(ptrtofree);
	ERROR_exit(errCode);
}