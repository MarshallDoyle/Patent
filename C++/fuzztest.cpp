#include <iostream>
#include "fuzzywuzzy.hpp"  // Use quotes for local header files

int main() {
    // Example usage of fuzz::ratio to compare two strings
    int score = fuzz::ratio("this is a test", "this is a test!");

    std::cout << "Similarity score: " << score << std::endl;

    return 0;
}
