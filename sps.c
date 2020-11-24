#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct CellStruct {
	char* cellText;
	unsigned sizeOfText;
}cell;

typedef struct Row {
	cell* row;
	unsigned numberOfCells;
}row;

typedef struct Table {
	row* table;
	unsigned numberOfRows;
}table;


/**
* Returns number of symbols
* before '\0'. So it returns
* size of line.
* In @text is nullptr -
* returns 0
*/
unsigned length(char* text) {
	if (text) {
		int i = 0;
		while (text[i] != '\0') {
			i++;
		}
		return i;
	}
	return 0;
}

/**
* If copy is empty line, it should be nullprt.
* Allocate memory for @copy
* pointer. Do not forget to free().
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
			for (unsigned i = 0; origin[i] ; i++) {
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
* Prefix function for KMP algorithm.
* Builds array with prefix function
* values for line @s. Resulted
* array is in @pi array.
* @n - size of line.
* ATTENTION:
* Size of @pi MUST be at least @n.
*/
void prefix_function(char* s, int* pi, int n) {
	pi[0] = 0;
	for (size_t i = 1; i < n; ++i)
	{
		int j = pi[i - 1];
		while ((j > 0) && (s[i] != s[j]))
			j = pi[j - 1];
		if (s[i] == s[j])
			++j;
		pi[i] = j;
	}
}

//TODO: update comments in algorithmKMP
bool algorithmKMP(char* str, char* obr, int* pi) {
	pi[0] = 0;

	int l;  
	for (l = 1; obr[l]; ++l)
	{
		int j = pi[l - 1];
		while ((j > 0) && (obr[l] != obr[j]))
			j = pi[j - 1];	        
		if (obr[l] == obr[j])  
			++j;
		pi[l] = j;
	}
	int j = 0; 
	for (int i = 0; str[i]; ++i)
	{
		while ((j > 0) && (str[i] != obr[j]))
			
			j = pi[j - 1];   

		if (str[i] == obr[j])
			++j;              
		if (j == l)
			return true;
	}
	return false;
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

void printRow(row* row, const char delim) {
	if (row != NULL) {
		for (unsigned i = 0; i < row->numberOfCells; i++) {
			if (row->row[i].cellText != NULL) 
				printf("%s", row->row[i].cellText);

			if (i < row->numberOfCells - 1)
				printf("%c", delim);

			else printf("\n");
		}
	}
}

void printTable(table* tab, const char delim) {
	if (tab != NULL) {
		for (unsigned i = 0; i < tab->numberOfRows; i++) {
			printRow(&tab->table[i], delim);
		}
	}
}



int main(int argc, char* argv[]) {
	(void)argc, (void)argv;
	return 0;
}