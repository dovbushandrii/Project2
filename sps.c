#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE_SIZE 10000
#define MAX_CELL_TEXT_SIZE 500


/////////////////////////////////TABLE STRUCTURES///////////////////////////////////
typedef struct CellStruct {
	char* cellText;
	unsigned sizeOfText;
}cell;

typedef struct RowStruct {
	cell* row;
	unsigned numberOfCells;
}row;

typedef struct TableStruct {
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
				result[i] = B[i - sizeA];
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


///////////////////////////NUMBER FUNCTIONS/////////////////////////////////////////
/**
* Returns number of decimal 
* digits in @number
*/
int digitCount(int number) {
	if (number) {
		unsigned result = 0;
		while (number) {
			number /= 10;
			result++;
		}
		return result;
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////


//////////////////////////CELL EDIT FUNCTIONS///////////////////////////////////////
/**
* Returns pointer on text is cell
* with coordinates (@x,@y).
* Return NULL if @_table is NULL
* @tab - pointer on table
*/
cell* getCellPtr(table* _table, const unsigned x, const unsigned y) {
	if (_table != NULL) {
		return &_table->table[y].row[x];
	}
	return NULL;
}

/**
* Copies text from @text to @_cell
* with coordinates (@x,@y), if 
* @_cell and @text aren't NULL.
*/
void setText(cell* _cell, char* text) {
	if (_cell != NULL && text != NULL) {
		copy(text, _cell->cellText);
		_cell->sizeOfText = length(text);
	}
}

/**
* Number(@number > 0) from @number tranforms
* to text and set it to @_cell
* with coordinates (@x,@y), if 
* @_cell isn't NULL.
*/
void setNumber(cell* _cell, int number) {
	if (_cell != NULL) {
		int digCount = digitCount(number);
		char* tmp = (char*)realloc(_cell->cellText, (digCount + 1) * sizeof(char));
		if (tmp != NULL) {
			_cell->sizeOfText = digCount;
			_itoa_s(number, _cell->cellText, digCount + 1, 10);
		}
		(void)number;
	}
}

/**
* Sets @_cell->sizeOfText = 0 and
* realloc-s @_cell->cellText to pointer
* on empty space, if @_cell isn't NULL
*/
void clear(cell* _cell) {
	if (_cell != NULL) {
		char* tmp = (char*)realloc(_cell->cellText, 0);
		_cell->cellText = tmp;
		_cell->sizeOfText = 0;
	}
}

/**
* Swaps texts between @a and @b
* cells if @a and @b aren't NULL
*/
void swap(cell* a, cell* b) {
	if (a != NULL && b != NULL) {
		cell tmp = *b;
		*b = *a;
		*a = tmp;
	}
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////////INSERT NEW ELEMENTS TO TABLE/////////////////////////////
/**
* Inserts @newCell to
* position @index.
* If @index <= 0, the column will
* be inserted to pos 0
* If @index >= @_row->numberOfCells - 1
* then the column will be inserted at the end.
*/
void insertColumnToRow(row* _row, cell newCell, unsigned index) {
	if (_row != NULL) {
		_row->numberOfCells++;
		cell* tmp = (cell*)realloc(_row->row, _row->numberOfCells * sizeof(row));
		if (tmp != NULL) {
			_row->row = tmp;

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

/**
* Inserts empty column to
* position @index.
* If @index <= 0, the column will
* be inserted to pos 0
* If @index >= @_row->numberOfCells - 1
* then the column will be inserted at the end.
*/
void insertEmptyColumnToRow(row* _row, unsigned index) {
	cell newCell;
	initializeCell(&newCell);
	insertColumnToRow(_row, newCell, index);
}

/**
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
			insertEmptyColumnToRow(&_table->table[i],index);
		}
	}
}

/**
* Inserts @row to 
* position @index.
* If @index <= 0, the row will
* be inserted to pos 0
* If @index >= @_table->numberOfRows - 1
* then the row will be inserted at the end.
*/
void insertRow(table* _table, row newRow, unsigned index) {
	if (_table != NULL) {
		_table->numberOfRows++;
		row* tmp = (row*)realloc(_table->table, _table->numberOfRows * sizeof(row));
		if (tmp != NULL) {
			_table->table = tmp;

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

/**
* Inserts empty row to
* position @index.
* If @index <= 0, the row will
* be inserted to pos 0
* If @index >= @_table->numberOfRows - 1
* then the row will be inserted at the end.
*/
void insertEmptyRow(table* _table, unsigned index) {
	row newRow;
	initializeRow(&newRow);
	//Set number on cell in row = column number
	if (_table->numberOfRows > 1) {
		for (unsigned i = 0; i < _table->table[0].numberOfCells; i++) {
			insertEmptyColumnToRow(&newRow, newRow.numberOfCells);
		}
	}
	insertRow(_table, newRow, index);
}
////////////////////////////////////////////////////////////////////////////////////


/////////////////////////APPEND NEW TABLE ELEMENTS TO END///////////////////////////
/**
* Appends new cell to row's end
*/
void appendColumnToRow(row* _row) {
	if (_row != NULL) {
		insertEmptyColumnToRow(_row, _row->numberOfCells);
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
		insertEmptyRow(_table, _table->numberOfRows);
	}
}
////////////////////////////////////////////////////////////////////////////////////


/////////////////////////DELETE TABLE ELEMENTS FROM TABLE/////////////////////////////
/*
* Removes row with index @index in @_table
*/
void deleteRow(table* _table, unsigned index) {
	if (_table != NULL) {
		/*index is unsigned, so check only 
		* if index >= _table->numberOfRows
		*/
		if (index >= _table->numberOfRows) {
			index = _table->numberOfRows - 1;
		}

		freeRow(&_table->table[index]);
		_table->numberOfRows--;

		for (unsigned i = index; i < _table->numberOfRows; i++) {
			_table->table[i] = _table->table[i + 1];
		}

		row* tmp = (row*)realloc(_table->table, _table->numberOfRows * sizeof(row));
		/*If _table->numberOfRows  = 0
		* tmp could be either NULL of some pointer.
		* Just in order to not cause problems
		* in freeTable functions, I assume that
		* NULL can be getted ONLY in case of
		* _table->numberOfRows  = 0.
		* So I wont check tmp != NULL
		* before setting tmp values to
		* _table->table.
		*/
		_table->table = tmp;
	}
}

/*
* Removes cell with index @index in @_row
*/
void deleteColumnInRow(row* _row, unsigned index) {
	if (_row != NULL) {
		/*index is unsigned, so check only
		* if index >= _row->numberOfCells
		*/
		if (index >= _row->numberOfCells) {
			index = _row->numberOfCells - 1;
		}

		freeCell(&_row->row[index]);
		_row->numberOfCells--;

		for (unsigned i = index; i < _row->numberOfCells; i++) {
			_row->row[i] = _row->row[i + 1];
		}
		
		cell* tmp = (cell*)realloc(_row->row, _row->numberOfCells * sizeof(cell));
		/*If _row->numberOfCells = 0
		* tmp could be either NULL of some pointer.
		* Just in order to not cause problems
		* in freeTable functions, I assume that
		* NULL can be getted ONLY in case of
		* _row->numberOfCells = 0.
		* So I wont check tmp != NULL
		* before setting tmp values to 
		* _row->row.
		*/
		_row->row = tmp;
	}
}

/*
* Removes column with index @index in table
*/
void deleteColumn(table* _table, unsigned index) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			deleteColumnInRow(&_table->table[i],index);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////


/////////////////////////DELETE TABLE ELEMENTS FROM END/////////////////////////////
/*
* Removes last row in table
*/
void popBackRow(table* _table) {
	if (_table != NULL) {
		deleteRow(_table, _table->numberOfRows);
	}
}

/*
* Removes last cell in row
*/
void popBackColumnInRow(row* _row) {
	if (_row != NULL) {
		deleteColumnInRow(_row, _row->numberOfCells);
	}
}

/*
* Removes last column in table
*/
void popBackColumn(table* _table) {
	if (_table != NULL) {
		for (unsigned i = 0; i < _table->numberOfRows; i++) {
			popBackColumnInRow(&_table->table[i]);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////TABLE NORMILIZE FUCNTIONS///////////////////////////////
/**
* Normalize column count for 
* all rows in table.
*/
void normalize(table* _table) {
	unsigned maxCountOfColumns = 0;
	for (unsigned i = 0; i < _table->numberOfRows; i++) {
		if (_table->table[i].numberOfCells > maxCountOfColumns)
			maxCountOfColumns = _table->table[i].numberOfCells;
	}
	for (unsigned i = 0; i < _table->numberOfRows; i++) {
		if (_table->table[i].numberOfCells < maxCountOfColumns) {
			while (_table->table[i].numberOfCells < maxCountOfColumns) {
				appendColumnToRow(&_table->table[i]);
			}
		}
	}
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


/////////////////////////////FILE MANAGMENT FUCNTIONS///////////////////////////////
/**
* Transforms char array @line to row,
* if @line is NULL returns empty row.
*/
row textLineToRow(char* line, const char delim) {
	row newRow;
	initializeRow(&newRow);
	if (line != NULL) {
		char cellText[MAX_CELL_TEXT_SIZE + 1];
		for (int i = 0, j = 0; 1; i++, j++) {
			if (j > MAX_CELL_TEXT_SIZE) return newRow;

			if (line[i] == delim || line[i] == '\0') {
				cellText[j] = '\0';
				cell newCell;
				initializeCell(&newCell);
				setText(&newCell, cellText);
				insertColumnToRow(&newRow, newCell, newRow.numberOfCells);
				j = -1;
				if (line[i] == '\0') break;
			}
			else {
				cellText[j] = line[i];
			}
		}
	}
	return newRow;
}

/**
* Reads table from file
* with name @nameOfFile
* and writes it to @_table.
* @delim - char that separates
*		   cells.
*/
void readTableFromFile(table* _table, const char delim, char* nameOfFile) {
	char line[MAX_LINE_SIZE + 1];
	FILE* fin = fopen(nameOfFile, "rt");
	if (fin != NULL) {
		for (unsigned i = 0; fscanf(fin, "%s", line) > 0; i++) {
			insertRow(_table, textLineToRow(line, delim), i);
		}
		normalize(_table);
		fclose(fin);
	}	
}


/**
* Prints @_row to file @fout.
* @delim - symbol that separates cells
*/
void writeRowToFile(row* _row, const char delim, FILE* fout) {
	if (_row != NULL) {
		for (unsigned i = 0; i < _row->numberOfCells; i++) {
			if (_row->row[i].sizeOfText > 0)
				fprintf(fout, "%s", _row->row[i].cellText);

			if (i < _row->numberOfCells - 1)
				fprintf(fout, "%c", delim);

			else fprintf(fout, "\n");
		}
	}
}

/*
* Prints @_table to file with name @nameOfFile.
* @delim - symbol that separates cells
*/
void writeTableToFile(table* _table, const char delim, char* nameOfFile) {
	FILE* fout = fopen(nameOfFile, "a+t");
	if (fout != NULL) {
		if (_table != NULL) {
			for (unsigned i = 0; i < _table->numberOfRows; i++) {
				writeRowToFile(&_table->table[i], delim, fout);
			}
		}
		fclose(fout);
	}

}
////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char* argv[]) {
	(void)argc, (void)argv;
	return 0;
}