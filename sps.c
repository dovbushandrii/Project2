#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/////////////////////////////////TABLE STRUCTURES///////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////TABLE MEMORY MANAGMENT//////////////////////////////////
/**
* If @_cell is not NULL, allocates
* memory for text and sets sizeOfText to 0.
*/
void initializeCell(cell* _cell) {
	if (_cell != NULL) {
		_cell->cellText = (char*)malloc(0);	//On purpose to use free().
		_cell->sizeOfText = 0;
	}
}

/**
* If @_row is not NULL, allocates
* memory for cells and sets numberOfCells to 0.
*/
void initializeRow(row* _row) {
	if (_row != NULL) {
		_row->row = (cell*)malloc(0);		//On purpose to use free().
		_row->numberOfCells = 0;
	}
}

/**
* If @_table is not NULL, allocates
* memory for rows and sets numberOfRows to 0.
*/
void initializeTable(table* _table) {
	if (_table != NULL) {
		_table->table = (row*)malloc(0);	//On purpose to use free().
		_table->numberOfRows = 0;
	}
}

/**
* Frees memory from @_cell->cellText.
* Sets @_cell->sizeOfText to 0.
*/
void freeCell(cell* _cell) {
	if (_cell != NULL) {
		free(_cell->cellText);
		_cell->sizeOfText = 0;
	}
}

/**
* Frees memory from @_row->row.
* Sets @_row->numberOfCells to 0.
*/
void freeRow(row* _row) {
	if (_row != NULL) {
		for (unsigned i = 0; i < _row->numberOfCells; i++) {
			freeCell(&_row->row[i]);
		}
		free(_row->row);
		_row->numberOfCells = 0;
	}
}

/**
* Frees memory from @_table->table.
* Sets @_table->numberOfRows to 0.
*/
void freeTable(table* _table) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			freeRow(&_table->table[i]);
		}
		free(_table->table);
		_table->numberOfRows = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////////TEXT FUNCTIONS///////////////////////////////////////////
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
		char* tmp;
		if (copy != NULL) {
			tmp = (char*)realloc(copy, (sizeOfText + 1) * sizeof(char));
		}
		else {
			tmp = (char*)malloc((sizeOfText + 1) * sizeof(char));
		}

		if (tmp != NULL) {
			copy = tmp;
			unsigned i = 0;
			for (; origin[i] ; i++) {
				copy[i] = origin[i];
			}
			copy[i] = '\0';
		}
	}
	else copy = NULL;
}

/**
* Concantination A + B. Allocate memory for new
* pointer. Do not forget to free().
*/
char* concantinate(char* A, char* B) {
	if (A != NULL && B != NULL) {
		unsigned sizeA = length(A);
		unsigned sizeB = length(B);
		char* result = (char*)malloc((sizeA + sizeB + 1) * sizeof(char));
		if (result != NULL) {
			unsigned i = 0;
			for (; i < sizeA; i++) {
				result[i] = A[i];
			}
			for (; i < sizeA + sizeB; i++) {
				result[i] = B[i - sizeA]; //Здесь указывает переполнение буфера
			}
			result[sizeA + sizeB] = '\0';
		}
		return result;
	}
}

/*
* Knuth-Morris-Pratt Algorithm.
* Returns true, if @obr line exists in @str line
* @pi - array of ints.
* ATTENTION:
* Size of @pi MUST be at least as size of @obr
*/
bool algorithmKMP(char* str, char* obr, int* pi) {
	/**
	* Prefix function for KMP algorithm.
	* Builds array with prefix function
	* values for line @obr. Resulted
	* array is in @pi array.
	*/
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
	//KMP algorithm
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
////////////////////////////////////////////////////////////////////////////////////


//////////////////////////GET SET CELL TEXT/////////////////////////////////////////
/**
* Returns pointer on text is cell
* with coordinates (@x,@y)
* @tab - pointer on table
*/
cell* getCellPtr(table* _table, const unsigned x, const unsigned y) {
	return &_table->table[y].row[x];
}

/**
* Copies text from @text to cell
* with coordinates (@x,@y)
* @tab - pointer on table
*/
void setCellText(table* _table, char* text, const unsigned x, const unsigned y) {
	cell* gettedCell = getCellPtr(_table, x, y);
	copy(text, gettedCell->cellText);
	gettedCell->sizeOfText = length(text);
}
////////////////////////////////////////////////////////////////////////////////////


/////////////////////////TABLE PRINTTING FUNCTIONS//////////////////////////////////
/**
* Prints @_row to stdout.
* @delim - symbol that separates cells
*/
void printRow(row* _row, const char delim) {
	if (_row != NULL) {
		for (unsigned i = 0; i < _row->numberOfCells; i++) {
			if (_row->row[i].sizeOfText > 0)
				printf("%s", _row->row[i].cellText);

			if (i < _row->numberOfCells - 1)
				printf("%c", delim);

			else printf("\n");
		}
	}
}

/*
* Prints @_table to stdout.
* @delim - symbol that separates cells
*/
void printTable(table* _table, const char delim) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			printRow(&_table->table[i], delim);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////////INSERT NEW ELEMENTS TO TABLE/////////////////////////////

/*
* Inserts empty column to
* position @index.
* If @index <= 0, the column will
* be inserted to pos 0
* If @index >= @_row->numberOfCells - 1
* then the column will be inserted at the end.
*/
void insertColumnToRow(row* _row, unsigned index) {
	if (_row != NULL) {
		_row->numberOfCells++;
		cell* tmp = (cell*)realloc(_row->row, _row->numberOfCells * sizeof(row));
		if (tmp != NULL) {
			_row->row = tmp;

			cell newCell;
			initializeCell(&newCell);

			unsigned i = _row->numberOfCells - 1;
			for (; i > 0 && i > index; i--) {
				_row->row[i] = _row->row[i - 1];
			}
			_row->row[i] = newCell;
		}
		else {
			_row->numberOfCells--;
		}
	}
}

/*
* Inserts empty column to
* position @index.
* If @index <= 0, the column will
* be inserted to pos 0
* If @index >= @_row->numberOfCells - 1
* then the column will be inserted at the end.
*/
void insertColumn(table* _table, unsigned index) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			insertColumnToRow(&_table->table[i],index);
		}
	}
}

/*
* Inserts empty row to 
* position @index.
* If @index <= 0, the row will
* be inserted to pos 0
* If @index >= @_table->numberOfRows - 1
* then the row will be inserted at the end.
*/
void insertRow(table* _table, unsigned index) {
	if (_table != NULL) {
		_table->numberOfRows++;
		row* tmp = (row*)realloc(_table->table, _table->numberOfRows * sizeof(row));
		if (tmp != NULL) {
			_table->table = tmp;

			row newRow;
			initializeRow(&newRow);
			//Set number on cell in row = column number
			if (_table->numberOfRows > 1) {
				for (unsigned i = 0; i < _table->table[0].numberOfCells; i++) {
					insertColumnToRow(&newRow,newRow.numberOfCells);
				}
			}

			unsigned i = _table->numberOfRows - 1;
			for (; i > 0 && i > index; i--) {
				_table->table[i] = _table->table[i - 1];
			}
			_table->table[i] = newRow;
		}
		else {
			_table->numberOfRows--;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////


/////////////////////////APPEND NEW TABLE ELEMENTS TO END///////////////////////////
/**
* Appends new cell to row's end
*/
void appendColumnToRow(row* _row) {
	if (_row != NULL) {
		insertColumnToRow(_row, _row->numberOfCells);
	}
}

/**
* Appensd new column to table
*/
void appendColumn(table* _table) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			appendColumnToRow(&_table->table[i]);
		}
	}
}

/**
* Appends new row to table
*/
void appendRow(table* _table) {
	if (_table != NULL) {
		insertRow(_table, _table->numberOfRows);
	}
}
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////DELETE TABLE ELEMENTS FROM END/////////////////////////////
/*
* Removes last row in table
*/
void popBackRow(table* _table) {
	if (_table != NULL) {
		freeRow(&_table->table[--(_table->numberOfRows)]);
		row* tmp = (row*)realloc(_table->table, _table->numberOfRows * sizeof(row));
		if(tmp != NULL)
			_table->table = tmp;	
	}
}

/*
* Removes last cell in row
*/
void popBackCellInRow(row* _row) {
	if (_row != NULL) {
		freeCell(&_row->row[--_row->numberOfCells]);
		cell* tmp = (cell*)realloc(_row->row, _row->numberOfCells * sizeof(cell));
		if (tmp != NULL)
			_row->row = tmp;
	}
}

/*
* Removes last column in table
*/
void popBackColumn(table* _table) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			popBackCellInRow(&_table->table[i]);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
	(void)argc, (void)argv;
	table newTable;
	initializeTable(&newTable);
	appendRow(&newTable);
	appendRow(&newTable);
	appendColumn(&newTable);
	appendColumn(&newTable);
	setCellText(&newTable, "A", 0, 0);
	setCellText(&newTable, "B", 1, 0);
	setCellText(&newTable, "C", 0, 1);
	setCellText(&newTable, "D", 1, 1);
	insertRow(&newTable, 10);
	setCellText(&newTable, "W", 0, 1);
	setCellText(&newTable, "V", 1, 1);
	insertColumn(&newTable, 3);
	printTable(&newTable, ':');
	printf("\n");
	popBackColumn(&newTable);
	printTable(&newTable, ':');
	printf("\n");
	popBackColumn(&newTable);
	printTable(&newTable, ':');
	freeTable(&newTable);
	return 0;
}