#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct CellStruct {
	char* cellText;
}cell;

typedef struct Row {
	cell* row;
}row;

typedef struct Table {
	row* table;
}table;

/**
* Returns number of symbols
* before '\0'. So it returns
* size of line.
* In @text is nullptr -
* returns -1
*/
unsigned length(char* text) {
	if (text) {
		int i = 0;
		while (text[i] != '\0') {
			i++;
		}
		return i;
	}
	return -1;
}

/**
* If copy is empty line, it should be nullprt
*/
void copy(char* origin, char* copy) {
	if (origin != NULL) {
		unsigned sizeOfText = length(origin);
		if (copy != NULL) {
			char* tmp = (char*)realloc(copy, (sizeOfText + 1) * sizeof(char));
			if (tmp != NULL) {
				copy = tmp;
			}

		}
		else {
			char* tmp = (char*)malloc((sizeOfText + 1) * sizeof(char));
			if (tmp != NULL) {
				copy = tmp;
			}
		}
		if (copy != NULL) {
			for (unsigned i = 0; i < sizeOfText + 1; i++) {
				copy[i] = origin[i];
			}
		}
		else copy = NULL;
	}
	else copy = NULL;
}

/**
* Concantination A + B. Allocate memory for new
* pointer. Do not forget to free().
*/
char* concantinate(char* A, char* B) {
	unsigned sizeA = length(A);
	unsigned sizeB = length(B);
	char* result = (char*)malloc((sizeA + sizeB + 1) * sizeof(char));
	if (result != NULL) {
		unsigned i = 0;
		for (; i < sizeA; i++) {
			result[i] = A[i];
		}
		for (; i < sizeA + sizeB; i++) {
			result[i] = B[i - sizeA];
		}
		result[sizeA + sizeB] = '\0';
	}
	return result;
}

/**
* Returns pointer on text is cell
* with coordinates (@x,@y)
* @tab - pointer on table
*/
char* getCellTextPtr(table* tab, const unsigned x, const unsigned y) {
	return tab->table[y].row[x].cellText;
}

/**
* Copies text from @text to cell
* with coordinates (@x,@y)
* @tab - pointer on table
*/
void setCellText(table* tab, char* text, const unsigned x, const unsigned y) {
	char* textInCell = getCellTextPtr(tab, x, y);
	copy(text, textInCell);
}

/*
* TODO -> KMP algorithm
*/
int main(int argc, char* argv[]) {

	return 0;
}