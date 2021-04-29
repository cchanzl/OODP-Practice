// tableOOP.c// CID:
// Degree: MSc Computing Science
// Module: 517 Object Oriented Design and Programming
//
// Add all of your code that pertains to question 2 to this file.

#include<iostream>
#include<string>
#include<cmath>
#include<vector>
#include<sstream>
#include<iomanip>

// The declarations of the class template ’vector’ are below.
/*
template <typename T> 
class vector {
public:
    vector(); // constructor that creates an empty vector
    void push_back(const T& item); // adds item to the vector
    vector<T>::constant_iterator cbegin(); // returns constant iterator
    vector<T>::constant_iterator cend(); // returns constant iterator
    unsigned int size(); // returns the number of items
};*/

// Available helper functions that can be used

/* Returns a std::string that is comprised of the given symbol with the given
 * length. E.g. makeErrorString(’#’, 3) returns the string: "###" */

std::string makeErrorString(char symbol, unsigned int length) {
    std::string s(length, symbol);
    return s;
};

/* Returns a std::string of the given number with the given decimal places. It
 * also rounds numbers up or down as appropriate. E.g. numberToString(10.9, 0)
 * returns the string: "11" */

std::string numberToString(float number, unsigned int decimal_places) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimal_places) << number;
    std::string s = stream.str();
    return s;
};

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

template<const char errorSymbol>
class Table {
private:
    const int row;
    const int col;
    int maxWidth;
    const char error = errorSymbol;
    std::vector<Cell*> tableVec;
    int nextRow = 0;
    int nextCol = 0;
    bool tableFull = false;

    // to print empty cell
    void printEmptyCell() {
        std::cout << " ";
    }

public:
    Table() {};
    Table(const int row, const int col, const int width)
        : row(row), col(col), maxWidth(width) {};

    void addCell(Cell* cell) {
        if (tableFull) {
            std::cout << "Table is full" << std::endl;
            return;
        }

        tableVec.push_back(cell);

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

        if (nextCol < col - 1) nextCol++;
   
    }

    void printTable() {
        // print each cell
        std::vector<Cell*>::const_iterator it;
        int count(0);
        for (it = tableVec.begin(); it != tableVec.end(); it++) {
            std::cout << "|";

            // check if cell is empty
            if (*it == nullptr) printEmptyCell();

            // if not empty
            (*it)->printCell(maxWidth, error); // via dynamic binding
            count++;

            // print end of column "|"
            if (count == col) {
                std::cout << "|" << std::endl;
                count = 0;
            }
        }
    }

    // deallocate memory
    /*
    virtual ~Table() {
        for (Cell* cell : tableVec) delete cell;
        tableVec.clear();
    }*/

}; // end table


class Currency : public Cell {
private:
    std::string currency;
    std::string value;
    int dp; // decimal place
    int padding = 0;

public:
    Currency(std::string currency, float value, int prefix=0, int postfix=0, int dp=0)
        : Cell(cell_width, prefix, postfix), currency(currency), dp(dp) {
        this->value = numberToString(value, dp);
    };

    virtual void printCell(const int max_col_width, const char error) override {
        if (isExceedWidth(max_col_width)) {
            std::cout << makeErrorString(error, max_col_width);
            return;
        }

        for (int i = 0; i < prefix; i++) {
            std::cout << " ";
        }

        std::cout << value << " " << currency;

        for (int i = 0; i < postfix; i++) {
            std::cout << " ";
        }

        for (int i = 0; i < padding; i++) {
            std::cout << " ";
        }

    }

    virtual bool isExceedWidth(const int width) override {
        int remainder = width;
        remainder -= prefix + postfix;
        remainder -= currency.size();
        remainder -= 1; // space between value and currency
        remainder -= value.size();
        if (remainder < 0) return true;
        padding = remainder;
        return false;
    }

};

class Text : public Cell {
private:
    std::string text;
    int padding = 0;

public:
    Text(std::string text, int prefix = 0, int postfix = 0, int dp = 0)
        : Cell(cell_width, prefix, postfix), text(text) {};

    virtual void printCell(const int max_col_width, const char error) override {
        if (isExceedWidth(max_col_width)) {
            std::cout << makeErrorString(error, max_col_width);
            return;
        }

        for (int i = 0; i < prefix; i++) {
            std::cout << " ";
        }

        std::cout << text;

        for (int i = 0; i < postfix; i++) {
            std::cout << " ";
        }

        for (int i = 0; i < padding; i++) {
            std::cout << " ";
        }

    }

    virtual bool isExceedWidth(const int width) override {
        int remainder = width;
        remainder -= prefix + postfix;
        remainder -= text.size();
        if (remainder < 0) return true;
        padding = remainder;
        return false;
    }

};

class Duration : public Cell {
private:
    int value;
    const int hours_to_minutes = 60;
    int hours = floor(value/hours_to_minutes);
    int minutes = value - hours*hours_to_minutes;
    int padding = 0;

public:
    Duration(int value, int prefix = 0, int postfix = 0)
        : Cell(cell_width, prefix, postfix), value(value) {};

    virtual void printCell(const int max_col_width, const char error) override {
        if (isExceedWidth(max_col_width)) {
            std::cout << makeErrorString(error, max_col_width);
            return;
        }

        for (int i = 0; i < prefix; i++) {
            std::cout << " ";
        }

        std::cout << hours << ":" << minutes;

        for (int i = 0; i < postfix; i++) {
            std::cout << " ";
        }

        for (int i = 0; i < padding; i++) {
            std::cout << " ";
        }

    }

    virtual bool isExceedWidth(const int width) override {
        int remainder = width;
        remainder -= prefix + postfix;
        remainder -= std::to_string(hours).size();
        remainder -= std::to_string(minutes).size();
        remainder -= 1; // for ':'
        if (remainder < 0) return true;
        padding = remainder;
        return false;
    }

};

// Question b

int main() {
    Table<'#'> table(3, 2, 10);
    table.addCell(new Text("Karting Time:"));
    table.addCell(new Text("Price:"));
    table.addCell(new Duration(85));
    table.addCell(new Currency("GBP", 60.50, 0, 0, 2));
    table.addCell(new Duration(25));
    table.addCell(new Currency("USD", 25.4, 3, 0, 0));
    table.printTable();
}