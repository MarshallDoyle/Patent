#include <cctype>   
#include <algorithm> 
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib> 
#include <limits>
#include "csv.h"
#include <map>

// Function declarations
std::string toLower(const std::string& str);
std::string strip(const std::string& str);
bool compareStrings(const std::string& str1, const std::string& str2);

std::string toLower(const std::string& str) {
    std::string lowerCaseStr;
    for (char c : str) {
        lowerCaseStr += std::tolower(static_cast<unsigned char>(c)); // Ensure correct overload
    }
    return lowerCaseStr;
}

std::string strip(const std::string& str) {
    auto start = std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });

    auto end = std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base();

    if (start >= end) return ""; // The string is all whitespace
    return std::string(start, end);
}

int binary_search(const std::vector<std::vector<std::string>>& data, int key, const std::string& value) {
    int low = 0;
    int high = static_cast<int>(data.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        std::string midValue = toLower(strip(data[mid][key]));

        if (midValue == toLower(strip(value))) {
            return mid; // Found the value
        } else if (midValue < toLower(strip(value))) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1; // Value not found
}

int levenshteinDistance(const std::string &s1, const std::string &s2) {
    const std::size_t len1 = s1.size(), len2 = s2.size();
    std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);

    for (unsigned int i = 0; i < prevCol.size(); i++)
        prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++) {
        col[0] = i + 1;
        for (unsigned int j = 0; j < len2; j++) {
            col[j + 1] = std::min({ 
                prevCol[j + 1] + 1, 
                col[j] + 1, 
                prevCol[j] + (s1[i] == s2[j] ? 0 : 1)
            });
        }
        col.swap(prevCol);
    }
    return prevCol[len2];
}

int binary_search_with_levenshtein(const std::vector<std::vector<std::string>>& data, int key, const std::string& value) {
    int low = 0;
    int high = static_cast<int>(data.size()) - 1;
    int closestMatch = -1;
    int closestDistance = std::numeric_limits<int>::max();

    while (low <= high) {
        int mid = low + (high - low) / 2;
        std::string midValue = toLower(strip(data[mid][key]));
        std::string searchValue = toLower(strip(value));

        if (midValue == searchValue) {
            return mid; // Found the exact match
        } else {
            // Calculate Levenshtein distance for a close match
            int distance = levenshteinDistance(midValue, searchValue);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestMatch = mid;
            }
            if (midValue < searchValue) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    // Return the closest match if no exact match was found
    return closestMatch;
}

int lengthScore(const std::string &str1, const std::string &str2, int levenshteinScore) {
    int deltaLength = std::abs(static_cast<int>(str1.length()) - static_cast<int>(str2.length()));
    return deltaLength * levenshteinScore;
}

bool compareStrings(const std::string& str1, const std::string& str2) {
    return toLower(strip(str1)) == toLower(strip(str2));
}

int main() {
    std::string str1 = " Hello, World! ";
    std::string str2 = "hello, world!";

    if (compareStrings(str1, str2)) {
        std::cout << "The strings are equal." << std::endl;
    } else {
        std::cout << "The strings are not equal." << std::endl;
    }

    // Using CSVReader to read a CSV file
    try {
        io::CSVReader<4> in("c:\\Users\\marsh\\Patent Tracking\\Sorted\\Sorted_assignee\\Sorted_Cleaned_assignee_assignee.csv");
        std::string col1, col2, col3, col4;

        // Reading the first row (assuming it's the header)
        if (in.read_row(col1, col2, col3, col4)) {
            std::cout << "CSV Header: " << col1 << ", " << col2 << ", " << col3 << std::endl;
        }

        // Reading and storing the first five values of the first column
        std::map<int, std::string> firstColumnData;
        int count = 0;
        while (count < 5 && in.read_row(col1, col2, col3, col4)) {
            firstColumnData[count] = col2;
            count++;
        }

        // Print the dictionary
        std::cout << "Second column data:" << std::endl;
        for (const auto& entry : firstColumnData) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error reading CSV: " << e.what() << std::endl;
    }

    // Expanded dataset with typos and variations
    std::vector<std::vector<std::string>> data = {
        {"apple", "fruit", "red"},
        {"banana", "fruit", "yellow"},
        {"banan", "fruit", "yellow"}, // Typo
        {"carrot", "vegetable", "orange"},
        {"date", "fruit", "brown"},
        {"eggplant", "vegetable", "purple"},
        {"bananna", "fruit", "yellow"}, // Typo
        {"pineapple", "fruit", "yellow"},
        {"tomato", "vegetable", "red"},
        {"bananana", "fruit", "yellow"} // Typo
    };

    // List of search terms
    std::vector<std::string> searchTerms = {"banana", "apple", "tomato", "pineaple", "grape"};

    for (const std::string& term : searchTerms) {
        int searchIndex = binary_search_with_levenshtein(data, 0, term);

        std::cout << "Search term: '" << term << "'" << std::endl;
        if (searchIndex != -1) {
            std::string foundWord = data[searchIndex][0];
            std::cout << "  Closest match: '" << foundWord << "' at index " << searchIndex << std::endl;
            
            int levDistance = levenshteinDistance(term, foundWord);
            int lenScore = lengthScore(term, foundWord, levDistance);
            std::cout << "  Levenshtein Distance: " << levDistance << ", Length Score: " << lenScore << std::endl;
        } else {
            std::cout << "  No close match found" << std::endl;
        }
    }

    return 0;
}
