#include "csv.h"
#include <iostream>
#include <string>
#include <vector>

// Function to load and print all rows from a CSV file
void loadAndPrintAllRows(const std::string& filename, int columnCount) {
    if (columnCount == 3) {
        io::CSVReader<3, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>, io::throw_on_overflow, io::no_comment> in(filename);
        std::string col1, col2, col3;
        while(in.read_row(col1, col2, col3)){
            std::cout << col1 << " " << col2 << " " << col3 << std::endl;
        }
    } else if (columnCount == 4) {
        io::CSVReader<4, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>, io::throw_on_overflow, io::no_comment> in(filename);
        std::string col1, col2, col3, col4;
        while(in.read_row(col1, col2, col3, col4)){
            std::cout << col1 << " " << col2 << " " << col3 << " " << col4 << std::endl;
        }
    } else {
        std::cerr << "Unsupported column count." << std::endl;
    }
}

int main() {
    std::string filename1 = "e://Patent Tracking//Sorted//Sorted_unique_assignee//Uncleaned.csv";
    std::string filename2 = "e://Patent Tracking//Sorted//Sorted_sec_cik//Uncleaned.csv";

    std::cout << "All rows of first CSV:" << std::endl;
    loadAndPrintAllRows(filename1, 4); // This file has four columns

    std::cout << "\nAll rows of second CSV:" << std::endl;
    loadAndPrintAllRows(filename2, 3); // This file has three columns

    return 0;
}
