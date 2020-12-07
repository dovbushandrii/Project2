#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE_SIZE 10000
#define MAX_CELL_TEXT_SIZE 500
#define MAX_COMMAND_SIZE 1000
#define UNDERSCORE -1			//Stands for '_'
#define NUMBER_OF_VARS 10

//https://github.com/dovbushandrii/Project2

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

typedef struct VariablesStruct {
	cell numVar[NUMBER_OF_VARS];
	cell** selVar;
}variables;
////////////////////////////////////////////////////////////////////////////////////

void start(int args, char* argv[]);

int main(int argc, char* argv[]) {
	start(argc, argv);
	return 0;
}


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

void initializeVariable(variables* vars) {
	if (vars != NULL) {
		for (int i = 0; i < NUMBER_OF_VARS; i++) {
			initializeCell(&vars->numVar[i]);
		}
		vars->selVar = (char**)malloc(0); //On purpose to use free().
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

void freeVariables(variables* vars) {
	if (vars != NULL) {
		for (int i = 0; i < NUMBER_OF_VARS; i++) {
			freeCell(&vars->numVar[i]);
		}
		free(vars->selVar);
	}
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////////NUMBER FUNCTIONS/////////////////////////////////////////
/**
* Returns number of decimal
* digits in @number
*/
int digitCount(long number) {
	if (number) {
		int result = 0;
		while (number) {
			number /= 10;
			result++;
		}
		return result;
	}
	return 1;
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
	if (text != NULL) {     //if(text) also OK
		int i = 0;
		while (text[i] != '\0') {
			i++;
		}
		return i;
	}
	return 0;
}

/**
* Checks if @text is a number.
* 0 - number
* 0,0 - number
* 0.0 - number
* .2 - number (0.2)
* . - number (0)
* 0,,0 - NOT a number
* 0..0 - NOT a number
* 
* If @text is NULL return
* false
*/
bool isThisLineANumber(char* text) {
	if (text != NULL) {		//if(text) also OK
		bool wasThereADot = false;
		for (unsigned i = 0; text[i] != '\0'; i++) {
			if (text[i] == '.' || text[i] == ',') {
				if (!wasThereADot)
					wasThereADot = true;
				else
					return false;
			}
			else if (text[i] != '-' && text[i] != 'e' && text[i] != '+' && (text[i] < '0' || text[i] >'9'))
				return false;
		}
		return true;
	}
	return false;
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
	return NULL;
}

/*
* Returns cutted line @result
* "line[from]-line[to]"
* 
* slice("abcde",result,1,3)
* result = "bcd"
*/
void slice(char* line,char* result, int from, int to)
{
	int j = 0;
	for (int i = from; i <= to; ++i)
		result[j++] = line[i];
	result[j] = '\0';
};

/*
* Knuth-Morris-Pratt Algorithm.
* Returns true, if @obr line exists in @str line
* @pi - array of ints.
* ATTENTION:
* Size of @pi MUST be at least as size of @obr
*/
bool algorithmKMP(char* str, char* obr) {
	/**
	* Prefix function for KMP algorithm.
	* Builds array with prefix function
	* values for line @obr. Resulted
	* array is in @pi array.
	*/
	int* pi = (int*)malloc(length(obr) * sizeof(int));
	if (pi != NULL) {
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
	else {
		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////////


//////////////////////////CELL EDIT FUNCTIONS///////////////////////////////////////
/**
* Returns pointer on text is cell
* with coordinates (@colInd,@rowInd).
* Return NULL if @_table is NULL
* @tab - pointer on table
*/
cell* getCellPtr(table* _table, const unsigned colInd, const unsigned rowInd) {
	if (_table != NULL) {
		return &_table->table[rowInd].row[colInd];
	}
	return NULL;
}

/**
* Returns index of row
* of @_cell in @_table
*/
int findRowOfCell(table* _table, cell* _cell) {
	for (unsigned r = 0; r < _table->numberOfRows; r++) {
		for (unsigned c = 0; c < _table->table->numberOfCells; c++) {
			if (_cell == getCellPtr(_table, c, r))
				return r;
		}
	}
}

/**
* Returns index of column
* of @_cell in @_table
*/
int findColumnOfCell(table* _table, cell* _cell) {
	for (unsigned r = 0; r < _table->numberOfRows; r++) {
		for (unsigned c = 0; c < _table->table->numberOfCells; c++) {
			if (_cell == getCellPtr(_table, c, r))
				return c;
		}
	}
}

/**
* Copies text from @text to @_cell
* with coordinates (@x,@y), if
* @_cell and @text aren't NULL.
*/
void setText(cell* _cell, char* text) {
	if (_cell != NULL && text != NULL) {
		if (text != NULL) {
			unsigned sizeOfText = length(text);
			char* tmp;
			if (_cell->cellText != NULL) {
				tmp = (char*)realloc(_cell->cellText, (sizeOfText + 1) * sizeof(char));
			}
			else {
				tmp = (char*)malloc((sizeOfText + 1) * sizeof(char));
			}
			if (tmp != NULL) {
				_cell->cellText = tmp;
				unsigned i = 0;
				for (; text[i]; i++) {
					_cell->cellText[i] = text[i];
				}
				_cell->cellText[i] = '\0';
				_cell->sizeOfText = sizeOfText;
			}
		}
		else {
			_cell->cellText = NULL;
			_cell->sizeOfText = 0;
		}
	}
}

/**
* Sets @_cell->sizeOfText = 0 and
* realloc-s @_cell->cellText to pointer
* on empty space, if @_cell isn't NULL
*/
void clearCell(cell* _cell) {
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
void swapCells(cell* a, cell* b) {
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
		if (_row->row != NULL) {
			cell* tmp = (cell*)realloc(_row->row, _row->numberOfCells * sizeof(row));
			if (tmp != NULL) {
				_row->row = tmp;

				unsigned i = _row->numberOfCells - 1;
				for (; i > 0 && i > index; i--) {
					_row->row[i] = _row->row[i - 1];
				}
				_row->row[i] = newCell;
				return;
			}
		}
	}
	printf("Memmory allocation problem");
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
		if (_table->table != NULL) {
			row* tmp = (row*)realloc(_table->table, _table->numberOfRows * sizeof(row));
			if (tmp != NULL) {
				_table->table = tmp;
				unsigned i = _table->numberOfRows - 1;
				for (;i > 0 && i > index; i--) {
					_table->table[i] = _table->table[i - 1];
				}
				_table->table[i] = newRow;
				return;
			}
		}
	}
	printf("Memmory allocation error");
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
	if (_table->numberOfRows > 0) {
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
/**
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
		for (unsigned i = 0; fscanf(fin, "%[^\n]\n", line) > 0; i++) {
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
	FILE* fout = fopen(nameOfFile, "w+t");
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


///////////////////////CELLS SELECTION FUNCTIONS////////////////////////////////////
/**
* [_,C] selection
* Returns array of pointers
* on table cells.
* Last element of array
* is NULL.
*/
cell** selectColumn(table* _table, int C) {
	if (_table != NULL) {

		unsigned countOfCells = _table->numberOfRows;
		cell** select = (cell**)malloc((countOfCells + 1) * sizeof(cell*));

		if (select != NULL) {
			for (unsigned i = 0; i < countOfCells; i++) {
				select[i] = getCellPtr(_table, C, i);
			}
			select[countOfCells] = NULL;
			return select;
		}
	}
	return NULL;
}

/**
* [R,_] selection
* Returns array of pointers
* on table cells.
* Last element of array
* is NULL.
*/
cell** selectRow(table* _table, int R) {
	if (_table != NULL) {

		unsigned countOfCells = _table->table[R].numberOfCells;
		cell** select = (cell**)malloc((countOfCells + 1) * sizeof(cell*));

		if (select != NULL) {
			for (unsigned i = 0; i < countOfCells; i++) {
				select[i] = getCellPtr(_table, i, R);
			}
			select[countOfCells] = NULL;
			return select;
		}
	}
	return NULL;
}

/**
* [R,C] selection
* Returns array of pointers
* on table cells.
* Last element of array
* is NULL.
*/
cell** selectCell(table* _table, int R, int C) {
	if (_table != NULL) {

		unsigned countOfCells = 1;
		cell** select = (cell**)malloc((countOfCells + 1) * sizeof(cell*));

		if (select != NULL) {
			select[0] = getCellPtr(_table, C, R);
			select[countOfCells] = NULL;
			return select;
		}
	}
	return NULL;
}

/**
* [_,_] selection
* Returns array of pointers
* on table cells.
* Last element of array
* is NULL.
*/
cell** selectTable(table* _table) {
	if (_table != NULL) {
		if (_table->numberOfRows > 0) {

			unsigned rowCount = _table->numberOfRows;
			unsigned colCount = _table->table[0].numberOfCells;
			unsigned countOfCells = rowCount * colCount;
			cell** select = (cell**)malloc((countOfCells + 1) * sizeof(cell*));

			if (select != NULL) {
				for (unsigned i = 0, r = 0; r < rowCount && i < countOfCells; r++) {	//It is not nessesary to write r < rowCount
					for (unsigned c = 0; c < colCount && i < countOfCells; c++, i++) {	//It is not nessesary to write i < countOfCells
						select[i] = getCellPtr(_table, c, r);
					}
				}
				select[countOfCells] = NULL;
				return select;
			}
		}
	}
	return NULL;
}

/**
* [R1,C2,R2,C2] selection
* Returns array of pointers
* on table cells.
* Last element of array
* is NULL.
*/
cell** selectWindow(table* _table, int R1, int C1, int R2, int C2) {
	if (_table != NULL) {

		if (R1 > (int)(_table->numberOfRows - 1)) {
			return NULL;
		}
		if (C1 > (int)(_table->table->numberOfCells - 1)) {
			return NULL;
		}
		if (R1 <= UNDERSCORE) {
			R1 = 0;
		}
		if (C1 <= UNDERSCORE) {
			C1 = 0;
		}
		if (R2 <= UNDERSCORE) {
			R2 = _table->numberOfRows - 1;
		}
		if (C2 <= UNDERSCORE) {
			C2 = _table->table->numberOfCells - 1;
		}

		int countOfCells = (R2 - R1 + 1) * (C2 - C1 + 1);
		cell** select = (cell**)malloc((countOfCells + 1) * sizeof(cell*));

		if (select != NULL) {
			for (int i = 0, r = R1; r <= R2 && i < countOfCells; r++) {	//It is not nessesary to write r < R2
				for (int c = C1; c <= C2 && i < countOfCells; c++, i++) {  //It is not nessesary to write i < countOfCells
					select[i] = getCellPtr(_table, c, r);
				}
			}
			select[countOfCells] = NULL;
			return select;
		}
	}
	return NULL;
}

/**
* Selects cell from @cells
* with minimum numeric value.
* If there were not any,
* returns only first cell
*/
cell** selectMin(cell** cells) {
	if (cells != NULL) {
		unsigned indexOfMin = 0;
		double minNumb = 0;
		bool minSet = false;
		for (unsigned i = 0; cells[i] != NULL; i++) {
			if (isThisLineANumber(cells[i]->cellText)) {
			double numb = atof(cells[i]->cellText);
			if (!minSet || numb < minNumb) {
				indexOfMin = i;
				minNumb = numb;
				minSet = true;
			}
			}
		}
		cells[0] = cells[indexOfMin];
		cells[1] = NULL;
	}
	return cells;
}

/**
* Selects cell from @cells
* with maximum numeric value.
* If there were not any,
* returns only first cell
*/
cell** selectMax(cell** cells) {
	if (cells != NULL) {
		unsigned indexOfMin = 0;
		double maxNumb = 0;
		bool minSet = false;
		for (unsigned i = 0; cells[i] != NULL; i++) {
			if (isThisLineANumber(cells[i]->cellText)) {
				double numb = atof(cells[i]->cellText);
				if (!minSet || numb > maxNumb) {
					indexOfMin = i;
					maxNumb = numb;
					minSet = true;
				}
			}
		}
		cells[0] = cells[indexOfMin];
		cells[1] = NULL;
	}
	return cells;
}

/**
* Selects first cell from @cells
* with cell text that include STR.
* If there were not any,
* returns only first cell
*/
cell** selectWithSTR(cell** cells, char* STR) {
	if (cells != NULL) {
		for (unsigned i = 0; cells[i] != NULL; i++) {
			if (algorithmKMP(cells[i]->cellText, STR)) {
				cells[0] = cells[i];
				cells[1] = NULL;
				return cells;
			}
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////
 

///////////////////////CELLS ARRAY EDIT FUNCTIONS///////////////////////////////////
/**
* Set text @STR for selected cells
*/
void set(cell** cells, char* STR) {
	if (cells != NULL) {
		for (unsigned i = 0; cells[i] != NULL; i++) {
			setText(cells[i], STR);
		}
	}
}

/**
* Clears text of selected cells
*/
void clear(cell** cells) {
	if (cells != NULL) {
		for (unsigned i = 0; cells[i] != NULL; i++) {
			clearCell(cells[i]);
		}
	}
}

/**
* Swaps every cell from @cells
* with @mainCell.
*/
void swap(cell** cells, cell* mainCell) {
	if (cells != NULL) {
		for (unsigned i = 0; cells[i] != NULL; i++) {
			swapCells(cells[i], mainCell);
		}
	}
}

/**
* Sets value of sum to cell @result
* Sum of all numeric values
* of cells if text in cell is in
* %lf format.
*/
void sum(cell** cells, cell* result) {
	if (result != NULL) {
		double sum = 0;
		for (unsigned i = 0; cells[i] != NULL; i++) {
			if (isThisLineANumber(cells[i]->cellText)) {
				double numb = 0;
				if(sscanf(cells[i]->cellText, "%lf", &numb))
					sum += numb;
				else {
					printf("isThisLineANumber function error");
					return;
				}
			}
		}
		char text[MAX_CELL_TEXT_SIZE + 1];
		sprintf(text,"%g", sum);
		setText(result, text);
	}
}

/**
* Sets value of average to cell @result
* Average of all numeric values
* of cells if text in cell is in
* %lf format.
*/
void avg(cell** cells, cell* result) {
	if (result != NULL) {
		double avg = 0;
		double count = 0;
		for (unsigned i = 0; cells[i] != NULL; i++) {
			if (isThisLineANumber(cells[i]->cellText)) {
				double numb = 0;
				if (sscanf(cells[i]->cellText, "%lf", &numb))
					avg += numb;
				else {
					printf("isThisLineANumber function error");
					return;
				}
				count++;
			}
		}
		avg /= count;
		char text[MAX_CELL_TEXT_SIZE + 1];
		sprintf(text, "%g", avg);
		setText(result, text);
	}
}

/**
* Counts not empty cells in @cells
* Result is written to @result cell
*/
void count(cell** cells, cell* result) {
	if (result != NULL) {
		double count = 0;
		for (unsigned i = 0; cells[i] != NULL; i++) {
			if (cells[i]->sizeOfText > 0) {
				count++;
			}
		}
		char text[MAX_CELL_TEXT_SIZE + 1];
		sprintf(text, "%g",count);
		setText(result, text);
	}
}

/*
* Sets size of text of cell from
* @cells to @result cell.
*/
void len(cell** cells, cell* result) {
	if (result != NULL) {
		unsigned count = 0;
		for (unsigned i = 0; cells[i] != NULL; i++) {
			count = cells[i]->sizeOfText;
		}
		char text[MAX_CELL_TEXT_SIZE + 1];
		sprintf(text, "%d", count);
		setText(result, text);
	}
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////SELECTION COMMANDS HANDLER///////////////////////////////////
/*
* Counts pointers on cells of @cells
* before NULL
*/
unsigned sizeOfSelect(cell** cells) {
	unsigned result = 0;
	for (unsigned i = 0; cells[i] != NULL; i++) {
		result++;
	}
	return result;
}

/*
* COPY MUST HAVE ALLOCATED MEMORY
* Returns copy of @cells.
*/
cell** setCellVar(cell** cells, cell** copy) {
	unsigned sizeOfCells = sizeOfSelect(cells);
	char** tmp = realloc(copy, (sizeOfCells + 1) * sizeof(char*));
	if (tmp != NULL) {
		copy = tmp;
		for (int i = 0; i < sizeOfCells; i++) {
			copy[i] = cells[i];
		}
		copy[sizeOfCells] = NULL;
		return copy;
	}
	printf("Memory allocation for varible error");
	return NULL;

}

/*
* Returns number of
* args in selection command
* by counting ','
*/
int argsCount(char* command) {
	int result = 1;
	for (int i = 0; command[i]; i++) {
		if (command[i] == ',')
			result++;
	}
	return result;
}

/*
* Handler for two-arg
* selection command
*/
cell** twoNumArgsSelect(table* _table, char* command) {
	char r[MAX_COMMAND_SIZE];
	char c[MAX_COMMAND_SIZE];
	int R = 0, C = 0;
	if (sscanf(command, "[%[^,],%[^]]", r, c)) {
		if (r[0] == '_') {
			if (c[0] == '_') {
				return selectTable(_table);
			}
			else {
				if (sscanf(command, "[%[^,],%d]", r, &C)) {
					return selectColumn(_table, C - 1);
				}
			}
		}
		else {
			if (c[0] == '_') {
				if (sscanf(command, "[%d,%[^]]", &R, c)) {
					return selectRow(_table, R - 1);
				}
			}
			else {
				if (sscanf(command, "[%d,%d]", &R, &C)) {
					return selectCell(_table,R - 1,C - 1);
				}
			}
		}
	}
	printf("Invalid selection command error\n");
	return NULL;
}

/*
* Handler for four-arg
* selection command
*/
cell** fourNumArgsSelect(table* _table, char* command) {
	char cr1[MAX_COMMAND_SIZE];
	char cc1[MAX_COMMAND_SIZE];
	char cr2[MAX_COMMAND_SIZE];
	char cc2[MAX_COMMAND_SIZE];
	int R1 = 0, C1 = 0, R2 = 0, C2 = 0;
	if (sscanf(command, "[%[^,],%[^,],%[^,],%[^]]", cr1, cc1, cr2, cc2)) {
		if (cr1[0] == '_') {
			R1 = UNDERSCORE;
		}
		else {
			if (sscanf(cr1, "%d", &R1)) {}
			else {
				printf("Invalid selection command error\n");
				return NULL;
			}
		}
		if (cc1[0] == '_') {
			C1 = UNDERSCORE;
		}
		else {
			if (sscanf(cc1, "%d", &C1)) {}
			else {
				printf("Invalid selection command error\n");
				return NULL;
			}
		}
		if (cr2[0] == '_') {
			R2 = UNDERSCORE;
		}
		else {
			if (sscanf(cr2, "%d", &R2)) {}
			else {
				printf("Invalid selection command error\n");
				return NULL;
			}
		}
		if (cc2[0] == '_') {
			C2 = UNDERSCORE;
		}
		else {
			if (sscanf(cc2, "%d", &C2)) {}
			else {
				printf("Invalid selection command error\n");
				return NULL;
			}
		}

		return selectWindow(_table, R1 - 1, C1 - 1, R2 - 1, C2 - 1);
	}
	printf("Invalid selection command error\n");
	return NULL;
}

/*
* Handle numeric selection commands
*/
cell** selNumHandler(table* _table, char* command) {
	if (argsCount(command) > 2) {
		return fourNumArgsSelect(_table, command);
	}
	else {
		return twoNumArgsSelect(_table, command);
	}
}

/*
* Selection commands handler
*/
cell** selComHandler(table* _table, cell** cells, variables* vars, char* command) {
	if (command != NULL && _table != NULL) {
		if (algorithmKMP(command,"[_]")) {
			cells = setCellVar(vars->selVar, cells);
			return cells;
		}
		else if (command[1] >= '0' && command[1] <= '9' || command[1] == '_') {
			return selNumHandler(_table, command);
		}
		else if (algorithmKMP(command, "min")) {
			return selectMin(cells);
		}
		else if (algorithmKMP(command, "max")) {
			return selectMax(cells);
		}
		else if (algorithmKMP(command, "find")) {
			char STR[MAX_COMMAND_SIZE];
			if (sscanf(command, "[find %[^]]", STR))
				return selectWithSTR(cells, STR);
		}
	}
	printf("Invalid selection command error\n");
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////CELL EDIT COMMANDS HANDLER//////////////////////////////
/*
* Handler for cell edit
* commands
*/
void celEdiComHandler(table* _table, cell** cells, char* command) {
	if (command != NULL && _table != NULL) {
		char com[6];
		if (sscanf(command, "%s", com)) {
			if (algorithmKMP(com, "set")) {
				char coor[MAX_COMMAND_SIZE + 1];
				if (sscanf(command, "%s %[^\n]", com, coor)) {
					coor[MAX_COMMAND_SIZE] = '\0';
					set(cells, coor);
					return;
				}
			}
			else if (algorithmKMP(com, "clear")) {
				clear(cells);
				return;
			}
			else {
				char coor[MAX_COMMAND_SIZE + 1];
				if (sscanf(command, "%s %s", com, coor)) {
					int r, c;
					coor[MAX_COMMAND_SIZE] = '\0';
					if (sscanf(coor, "[%d,%d]", &r, &c)) {
						if (algorithmKMP(com, "swap")) {
							swap(cells, getCellPtr(_table, c - 1, r - 1));
							return;
						}
						else if (algorithmKMP(com, "sum")) {
							sum(cells, getCellPtr(_table, c - 1, r - 1));
							return;
						}
						else if (algorithmKMP(com, "avg")) {
							avg(cells, getCellPtr(_table, c - 1, r - 1));
							return;
						}
						else if (algorithmKMP(com, "count")) {
							count(cells, getCellPtr(_table, c - 1, r - 1));
							return;
						}
						else if (algorithmKMP(com, "len")) {
							len(cells, getCellPtr(_table, c - 1, r - 1));
							return;
						}
					}
				}
			}
			
		}
	}
	printf("Invalid cell edit command error\n");
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////////TABLE EDIT COMMANDS HANDLER//////////////////////////////
/*
* Returns last cell 
* in @cells
*/
cell* getLast(cell** cells) {
	unsigned i = 0;
	while (cells[i] != NULL)
		i++;
	return cells[i - 1];
}

/*
* Handler for table
* edit commands.
* @_table - table
* @cells - selected cells
* @command - line with command
*/
void tabEdiComHandler(table* _table, cell** cells, char* command) {
	if (command != NULL && _table != NULL) {
		char com[5];
		if (sscanf(command, "%s", com)) {
			if (algorithmKMP(com, "row")) {
				if (cells != NULL) {
					int startRowInd = findRowOfCell(_table, cells[0]);
					int endRowInd = findRowOfCell(_table, getLast(cells));
					if (algorithmKMP(com, "i")) {
						insertEmptyRow(_table, startRowInd);
					}
					else if (algorithmKMP(com, "a")) {
						insertEmptyRow(_table, endRowInd + 1);
					}
					else if (algorithmKMP(com, "d")) {
						for (int i = endRowInd; i >= startRowInd; i--) {
							deleteRow(_table, i);
						}
					}
				}
				else {
					if (!algorithmKMP(com, "d")) {
						appendRow(_table);
					}
				}
				return;
			}
			else if (algorithmKMP(com, "col")) {
				if (cells != NULL) {
					int startColInd = findColumnOfCell(_table, cells[0]);
					int endColInd = findColumnOfCell(_table, getLast(cells));
					if (algorithmKMP(com, "i")) {
						insertColumn(_table, startColInd);
					}
					else if (algorithmKMP(com, "a")) {
						insertColumn(_table, endColInd + 1);
					}
					else if (algorithmKMP(com, "d")) {
						for (int i = endColInd; i >= startColInd; i--) {
							deleteColumn(_table, i);
						}
					}
				}
				else {
					if (!algorithmKMP(com, "d")) {
						appendColumn(_table);
					}
				}
				return;
			}
		}
	}
	printf("Invalid table edit command error\n");
}
////////////////////////////////////////////////////////////////////////////////////


//////////////////////////VARIABLE EDIT COMMANDS HANDLER////////////////////////////
void varEdiComHandler(table* _table, cell** cells, variables* vars, char* command) {
	if(_table != NULL && cells != NULL && command != NULL){
		char com[6];
		if (sscanf(command, "%s", com)) {
			com[5] = '\n';
			 if (algorithmKMP(com, "set")) {
				 vars->selVar = setCellVar(cells, vars->selVar);
				 return;
			 }
			 else {
				 int varInd;
				 if (sscanf(command, "%s _%d", com, &varInd)) {
					 if (algorithmKMP(com, "def")) {
						 setText(&vars->numVar[varInd], getLast(cells)->cellText);
						 return;
					 }
					 else if (algorithmKMP(com, "use")) {
						 for (unsigned i = 0; cells[i]; i++) {
							 setText(cells[i], vars->numVar[varInd].cellText);
						 }
						 return;
					 }
					 else if (algorithmKMP(com, "inc")) {
						 if (isThisLineANumber(vars->numVar[varInd].cellText)) {
							 double var;
							 if (sscanf(vars->numVar[varInd].cellText, "%lf", &var)) {
								 var++;
								 char text[MAX_CELL_TEXT_SIZE + 1];
								 sprintf(text, "%g", var);
								 setText(&vars->numVar[varInd], text);
								 return;
							 }
						 }
						 else {
							 setText(&vars->numVar[varInd], "1");
							 return;
						 }
						 
					 }
				 }
			 }
		}
	}
	printf("Invalid variable edit command\n");
}
////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////MAIN COMMANDS HANDLER////////////////////////////////

/*
* Checks is @command
* a table selection command.
*/
bool isThisSelection(char* command) {
	if (command != NULL) {
		if (command[0] == '[')
			return true;
	}
	return false;
}

/*
* Checks is @command
* a table edit command.
*/
bool isThisTableEdit(char* command) {
	if (command != NULL) {
		char com[6];
		if (sscanf(command, "%s", com)) {
			com[5] = '\0';
			if (algorithmKMP(com, "row") || algorithmKMP(com, "col"))
				return true;
		}
	}
	return false;
}

/*
* Checks is @command 
* a variable edit command.
*/
bool isThisVarEdit(char* command) {
	if (algorithmKMP(command, "def"))
		return true;
	else if (algorithmKMP(command, "use")) 
		return true;
	else if (algorithmKMP(command, "inc")) 
		return true;
	else if (algorithmKMP(command, "[set]")) 
		return true;
	return false;
}

/*
* Handles command and 
* call corresponding
* command handlers
*/
cell** handleComand(table* _table, cell** select, char* command, variables* vars) {
	if (isThisVarEdit(command)) {
		varEdiComHandler(_table, select, vars, command);
		return select;
	}
	else if (isThisSelection(command)) {
		return selComHandler(_table, select, vars, command);
	}
	else if (isThisTableEdit(command)) {
		tabEdiComHandler(_table, select, command);
		return select;
	}
	else {
		celEdiComHandler(_table, select,command);
		return select;
	}
}

/**
* Divides CMD_SEQUENCE to
* separate commads and 
* send command to @handleComand.
* @_table - table
* @commands - CMD_SEQUENCE
*/
void mainHandler(table* _table, char* commands) {
	cell** select = NULL;
	variables vars;
	initializeVariable(&vars);
	unsigned startOfCom = 0;
	char command[MAX_COMMAND_SIZE + 1];
	for (int j = 0; commands[j]; j++) {
		if (commands[j] == ';' || !commands[j + 1]) {
			if (!commands[j + 1]) j++;
			slice(commands, command, startOfCom, j - 1);
			startOfCom = j + 1;
			select = handleComand(_table, select, command, &vars);
		}
	}
	freeVariables(&vars);
	free(select);
}
////////////////////////////////////////////////////////////////////////////////////


void start(int args, char* argv[]) {
	char delim = ' ';
	if (args > 1) {
		table t;
		initializeTable(&t);

		unsigned wasThereDelim = 0;
		if (algorithmKMP(argv[1], "-d")) {
			delim = argv[2][0];
			wasThereDelim = 2;
		}

		readTableFromFile(&t, delim, argv[2 + wasThereDelim]);
		mainHandler(&t, argv[1 + wasThereDelim]);
		writeTableToFile(&t, delim, argv[2 + wasThereDelim]);
		freeTable(&t);
	}
	else {
		printf("Empty input error");
	}
}