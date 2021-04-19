// tableOOP.c// CID:
// Degree: MSc Computing Science
// Module: 517 Object Oriented Design and Programming
//
// Add all of your code that pertains to question 2 to this file.

#include<iostream>
#include<string>
#include<cmath>

// The declarations of the class template ’vector’ are below.

template <typename T> 
class vector {
public:
    vector(); // constructor that creates an empty vector
    void push_back(const T& item); // adds item to the vector
    vector<T>::constant_iterator cbegin(); // returns constant iterator
    vector<T>::constant_iterator cend(); // returns constant iterator
    unsigned int size(); // returns the number of items
};

// Available helper functions that can be used

/* Returns a std::string that is comprised of the given symbol with the given
 * length. E.g. makeErrorString(’#’, 3) returns the string: "###" */

std::string makeErrorString(char symbol, unsigned int length);

/* Returns a std::string of the given number with the given decimal places. It
 * also rounds numbers up or down as appropriate. E.g. numberToString(10.9, 0)
 * returns the string: "11" */

std::string numberToString(float number, unsigned int decimal_places);

/* Add your code below this line. */

// Question a

// abstract class Cell

class Cell {
protected:
    int cell_width;
    int prefix;
    int postfix;

public:
    Cell(int cell_width, int prefix, int postfix)
        : cell_width(cell_width), prefix(prefix), postfix(postfix) {};

    void setCellWidth(const int width) {
        cell_width = width;
    }

    int getWidth() {
        return cell_width;
    }

    virtual void printCell(const int max_col_width, const char error) = 0;
    virtual bool isExceedWidth(const int width) = 0;

};

template<const char error>
class Table {
private:
    const int row;
    const int col;
    int width;
    const char error = error;
    Cell* tableArray[row][col];
    int max_col_width[col];
    int nextRow = 0;
    int nextCol = 0;
    bool tableFull = false;

    // to print empty cell
    void printEmptyCell() {
        std::cout
    }

    // to find the maximum column width in a table
    void findMaxWidth() {
        for (int c = 0; c < row; c++) {
            int max_width = width;
            for (int r = 0; r < col; r++) {
                if ( !tableArray[r][c] ) continue;
                max_width = max(tableArray[r][c]->getWidth, max_width);
            }
            max_col_width[c] = max_width;
        }
    }

public:
    Table() {};
    Table(const int row, const int col, const int width)
        : row(row), col(col), width(width) {
        // initialise table as nullptr
        for (int r = 0; r < row; r++) {
            for (int c = 0; c < col; c++) {
                tableArray[r][c] = nullptr;
            }
        }
    }

    void addCell(const Cell* cell) {
        if (tableFull) {
            std::cout << "Table is full" << std::endl;
            return;
        }

        tableArray[nextRow][nextCol] = cell;
        
        // at last row and last column currently
        if (nextRow == row-1 && nextCol == col-1) {
            tableFull = true;
            return;
        }

        // reach end of col
        if (nextCol = col-1 && nextRow < row-1) {
            nextCol = 0;
            nextRow++;
            return;
        }

        // reach last row
        if (nextRow = row-1 && nextCol < col-1) {
            nextCol++;
            return;
        }

        if (nextCol < col-1) nextCol++;
    }

    void printTable() {
        // update max width for each column
        findMaxWidth();

        // print each cell
        for (int r = 0; r < row; r++) {
            std::cout << "|";
            for (int c = 0; c < col; c++) {
                // check if cell is empty
                if (!tableArray[r][c]) printEmptyCell();

                // if not empty
                tableArray[r][c]->printCell(max_col_width[c], error); // via dynamic binding
            }
            // print end of column "|"
            std::cout << "|" << std::endl;
        }
    }

    // deallocate memory
    virtual ~Table() {
        delete[][] tableArray;
    }

}; // end table


class Currency : public Cell {
private:
    std::string currency;
    int value;
    int dp; // decimal place

public:
    Currency(std::string currency, int value, int prefix=0, int postfix=0, int dp=0)
        : Cell(cell_width, prefix, postfix), currency(currency), value(value), dp(dp) {};

    virtual void printCell(const int max_col_width, const char error) override {
        if (isExceedWidth(max_col_width)) {
            makeErrorString(error, max_col_width);
            return;
        }

        for (int i = 0; i < prefix; i++) {
            std::cout << " ";
        }

        std::cout << numberToString(value, dp) << " " << currency;

        for (int i = 0; i < postfix; i++) {
            std::cout << " ";
        }

    }

    virtual bool isExceedWidth(const int width) override {
        int remainder = width;
        remainder -= prefix + postfix;
        remainder -= currency.size();
        remainder -= 1; // space between value and currency
        remainder -= std::to_string(value).size();
        if (remainder < 0) return true;
        return false;
    }

};

class Text : public Cell {
private:
    std::string text;

public:
    Text(std::string text, int prefix = 0, int postfix = 0, int dp = 0)
        : Cell(cell_width, prefix, postfix), text(text) {};

    virtual void printCell(const int max_col_width, const char error) override {
        if (isExceedWidth(max_col_width)) {
            makeErrorString(error, max_col_width);
            return;
        }

        for (int i = 0; i < prefix; i++) {
            std::cout << " ";
        }

        std::cout << text;

        for (int i = 0; i < postfix; i++) {
            std::cout << " ";
        }

    }

    virtual bool isExceedWidth(const int width) override {
        int remainder = width;
        remainder -= prefix + postfix;
        remainder -= text.size();
        if (remainder < 0) return true;
        return false;
    }

};

class Duration : public Cell {
private:
    int value;
    const int hours_to_minutes = 60;
    int hours = floor(value/hours_to_minutes);
    int minutes = value - hours*hours_to_minutes;

public:
    Duration(int value, int prefix = 0, int postfix = 0)
        : Cell(cell_width, prefix, postfix), value(value) {};

    virtual void printCell(const int max_col_width, const char error) override {
        if (isExceedWidth(max_col_width)) {
            makeErrorString(error, max_col_width);
            return;
        }

        for (int i = 0; i < prefix; i++) {
            std::cout << " ";
        }

        std::cout << hours << ":" << minutes;

        for (int i = 0; i < postfix; i++) {
            std::cout << " ";
        }

    }

    virtual bool isExceedWidth(const int width) override {
        int remainder = width;
        remainder -= prefix + postfix;
        remainder -= std::to_string(value).size();
        if (remainder < 0) return true;
        return false;
    }

};

// Question b

int main() {
    Table<'#'> table(3, 2, 15);
    table.addCell(new Text("Karting Time:"));
    table.addCell(new Text("Price"));
    table.addCell(new Duration(85));
    table.addCell(new Currency("GBP", 60.5, 0, 0, 2));
    table.addCell(new Duration(25));
    table.addCell(new Currency("USD", 25.4, 3, 0, 0));
    table.printTable();
}