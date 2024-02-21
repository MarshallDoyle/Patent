#include "csv.h" // Make sure this path is correct based on your project setup
#include <iostream>
#include <vector>
#include <string>

// Function to load the first row from a CSV file, adjusted for a flexible number of columns
std::vector<std::string> loadFirstRow(const std::string& filename, int columnCount) {
    std::vector<std::string> firstRow;
    
    if (columnCount == 3) {
        io::CSVReader<3, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>, io::throw_on_overflow, io::no_comment> in(filename);
        std::string col1, col2, col3;
        if(in.read_row(col1, col2, col3)){
            firstRow = {col1, col2, col3};
        }
    } else if (columnCount == 4) {
        io::CSVReader<4, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>, io::throw_on_overflow, io::no_comment> in(filename);
        std::string col1, col2, col3, col4;
        if(in.read_row(col1, col2, col3, col4)){
            firstRow = {col1, col2, col3, col4};
        }
    } else {
        std::cerr << "Unsupported column count." << std::endl;
    }
    
    return firstRow;
}

// Function to print the contents of a row
void printRow(const std::vector<std::string>& row) {
    for (const auto& col : row) {
        std::cout << col << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::string filename1 = "e://Patent Tracking//Sorted//Sorted_unique_assignee//Uncleaned.csv"; // Update this path
    std::string filename2 = "e://Patent Tracking//Sorted//Sorted_sec_cik//Uncleaned.csv"; // Update this path

    // Load and print the first row from each CSV file, specifying the correct column count
    auto firstRow1 = loadFirstRow(filename1, 4); // This file has four columns
    auto firstRow2 = loadFirstRow(filename2, 3); // This file has three columns

    std::cout << "First row of first CSV:" << std::endl;
    printRow(firstRow1);
    
    std::cout << "First row of second CSV:" << std::endl;
    printRow(firstRow2);

    return 0;
}
