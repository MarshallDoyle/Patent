#include <cctype>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include "csv.h"
#include <map>

// Function declarations
std::string toLower(const std::string& str);
std::string strip(const std::string& str);
bool compareStrings(const std::string& str1, const std::string& str2);
int binary_search(const std::vector<std::vector<std::string>>& data, int key, const std::string& value);
int levenshteinDistance(const std::string &s1, const std::string &s2);
int binary_search_with_levenshtein(const std::vector<std::vector<std::string>>& data, int key, const std::string& value);
int lengthScore(const std::string &str1, const std::string &str2, int levenshteinScore);

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

// Custom CSV Writer Function
void writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

void fullMatching(std::vector<std::vector<std::string>>& assignees, std::vector<std::vector<std::string>>& data) {
    // Loop through assignees and perform matching with data
    for (size_t i = 0; i < assignees.size(); ++i) {
        const auto& assigneeRow = assignees[i];
        std::string assignee = assigneeRow[1]; // Using Uncleaned Assignee for matching
        int searchIndex = binary_search(data, 0, assignee);
        int levenshteinScore = -1;
        int lengthScoreVal = -1;
        std::vector<std::string> row;

        if (searchIndex != -1) {
            // Exact match found
            levenshteinScore = levenshteinDistance(toLower(strip(assignee)), toLower(strip(data[searchIndex][0])));
            lengthScoreVal = lengthScore(assignee, data[searchIndex][0], levenshteinScore);

            // Prepare row for matched data
            row = {data[searchIndex][0], data[searchIndex][1], data[searchIndex][2], assignee, "Exact", std::to_string(levenshteinScore), std::to_string(lengthScoreVal)};
            matchedData.push_back(row);
            std::cout << "Matched row #" << i+1 << ": " << assignee << " with " << data[searchIndex][0] << " using exact match." << std::endl;

        } else {
            // Closest match
            searchIndex = binary_search_with_levenshtein(data, 0, assignee);
            if (searchIndex != -1) {
                levenshteinScore = levenshteinDistance(toLower(strip(assignee)), toLower(strip(data[searchIndex][0])));
                lengthScoreVal = lengthScore(assignee, data[searchIndex][0], levenshteinScore);

                // Prepare row for no match data
                row = {data[searchIndex][0], data[searchIndex][1], data[searchIndex][2], assignee, "No Match", std::to_string(levenshteinScore), std::to_string(lengthScoreVal)};
                noMatchData.push_back(row);
                std::cout << "Matched row #" << i+1 << ": " << assignee << " with " << data[searchIndex][0] << " using closest match." << std::endl;
            } else {
                std::cout << "Did not match row #" << i+1 << ": " << assignee << std::endl;
            }
        }
    }
}


// Function to read the data to be matched
std::vector<std::vector<std::string>> readData(const std::string& filePath, const int numColumns) {
    std::vector<std::vector<std::string>> data;
    try {
        io::CSVReader<numColumns, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>> dataReader(filePath);
        std::vector<std::string> row;
        while (dataReader.read_row(row)) {
            data.push_back(row);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading data: " << e.what() << std::endl;
    }
    return data;
}

int main() {
    std::vector<std::vector<std::string>> csrpUncleanedData = readData("c:\\Users\\marsh\\Patent Tracking\\Sorted\\Sorted_crsp_permo\\CSRP_Permco_Sorted_by_UnCleaned.csv", 3);
    std::vector<std::vector<std::string>> csrpCleanedData = readData("c:\\Users\\marsh\\Patent Tracking\\Sorted\\Sorted_crsp_permo\\CSRP_Permco_Sorted_by_Cleaned.csv", 3);
    std::vector<std::vector<std::string>> uniqueAssigneesUncleaned = readData("c:\\Users\\marsh\\Patent Tracking\\Sorted\\Sorted_unique_assignee\\Unique_Assignee_sorted_by_Cleaned.csv", 4);
    std::vector<std::vector<std::string>> uniqueAssigneesCleaned = readData("c:\\Users\\marsh\\Patent Tracking\\Sorted\\Sorted_unique_assignee\\Unique_Assignee_sorted_by_Uncleaned.csv", 4);

    // Create a CSV file for output
    std::ofstream outputFile("matching_results.csv");
    outputFile << "CSRP Uncleaned Company Name,CSRP Patnum,CSRP Cleaned Company Name,CSRP Cleaned Assignee,Unique Assignee pgpub_id,Unique Assignee Uncleaned Assignee,Unique Assignee Cleaned Assignee,Unique Assignee Patnum,Match Type,Levenshtein Score,Length Score\n";

    // Create vectors to store the matched and non-matched data
    std::vector<std::vector<std::string>> matchedData;
    std::vector<std::vector<std::string>> noMatchData;
  
    int exactMatches = 0;
    int fuzzMatches = 0;
    int noMatches = 0;

    // Iterate through CSRP data
    for (const auto& csrpRow : csrpUncleanedData) {
        std::string csrpUncleanedCompany = csrpRow[0];
        std::string csrpCleanedCompany = csrpRow[2];

        // Step 1: Exact match of "CSRP Uncleaned Company Name" to "Unique Assignee Uncleaned Assignee"
        int exactUncleanedIndex = binary_search(uniqueAssigneesUncleaned, 1, csrpUncleanedCompany);
        if (exactUncleanedIndex != -1) {
            // Record the match as "Exact"
            outputFile << csrpUncleanedCompany << "," << csrpRow[1] << "," << csrpCleanedCompany << ","
                       << csrpRow[3] << "," << uniqueAssigneesUncleaned[exactUncleanedIndex][2] << ","
                       << uniqueAssigneesUncleaned[exactUncleanedIndex][1] << ","
                       << uniqueAssigneesCleaned[exactUncleanedIndex][3] << ","
                       << uniqueAssigneesUncleaned[exactUncleanedIndex][0] << ",Exact,-,-\n";
            exactMatches++;
        } else {
            // Step 2: Exact match of "CSRP Cleaned Company Name" to "Unique Assignee Cleaned Assignee"
            int exactCleanedIndex = binary_search(uniqueAssigneesCleaned, 3, csrpCleanedCompany);
            if (exactCleanedIndex != -1) {
                // Record the match as "Exact"
                outputFile << csrpUncleanedCompany << "," << csrpRow[1] << "," << csrpCleanedCompany << ","
                           << csrpRow[3] << "," << uniqueAssigneesCleaned[exactCleanedIndex][2] << ","
                           << uniqueAssigneesUncleaned[exactCleanedIndex][1] << ","
                           << uniqueAssigneesCleaned[exactCleanedIndex][3] << ","
                           << uniqueAssigneesCleaned[exactCleanedIndex][0] << ",Exact,-,-\n";
                exactMatches++;
            } else {
                // Step 3: Closest match of "CSRP Uncleaned Company Name" to "Unique Assignee Uncleaned Assignee"
                int closestUncleanedIndex = binary_search_with_levenshtein(uniqueAssigneesUncleaned, 1, csrpUncleanedCompany);
                if (closestUncleanedIndex != -1) {
                    int levenshteinScore = levenshteinDistance(csrpUncleanedCompany, uniqueAssigneesUncleaned[closestUncleanedIndex][1]);
                    int lengthScoreVal = lengthScore(csrpUncleanedCompany, uniqueAssigneesUncleaned[closestUncleanedIndex][1], levenshteinScore);
                    // Record the match as "Fuzz"
                    outputFile << csrpUncleanedCompany << "," << csrpRow[1] << "," << csrpCleanedCompany << ","
                               << csrpRow[3] << "," << uniqueAssigneesUncleaned[closestUncleanedIndex][2] << ","
                               << uniqueAssigneesUncleaned[closestUncleanedIndex][1] << ","
                               << uniqueAssigneesCleaned[closestUncleanedIndex][3] << ","
                               << uniqueAssigneesUncleaned[closestUncleanedIndex][0] << ",Fuzz,"
                               << levenshteinScore << "," << lengthScoreVal << "\n";
                    fuzzMatches++;
                } else {
                    // Step 4: Closest match of "CSRP Cleaned Company Name" to "Unique Assignee Cleaned Assignee"
                    int closestCleanedIndex = binary_search_with_levenshtein(uniqueAssigneesCleaned, 3, csrpCleanedCompany);
                    if (closestCleanedIndex != -1) {
                        int levenshteinScore = levenshteinDistance(csrpCleanedCompany, uniqueAssigneesCleaned[closestCleanedIndex][3]);
                        int lengthScoreVal = lengthScore(csrpCleanedCompany, uniqueAssigneesCleaned[closestCleanedIndex][3], levenshteinScore);
                        // Record the match as "Fuzz"
                        outputFile << csrpUncleanedCompany << "," << csrpRow[1] << "," << csrpCleanedCompany << ","
                                   << csrpRow[3] << "," << uniqueAssigneesCleaned[closestCleanedIndex][2] << ","
                                   << uniqueAssigneesUncleaned[closestCleanedIndex][1] << ","
                                   << uniqueAssigneesCleaned[closestCleanedIndex][3] << ","
                                   << uniqueAssigneesCleaned[closestCleanedIndex][0] << ",Fuzz,"
                                   << levenshteinScore << "," << lengthScoreVal << "\n";
                        fuzzMatches++;
                    } else {
                        // Step 5: No match found
                        outputFile << csrpUncleanedCompany << "," << csrpRow[1] << "," << csrpCleanedCompany << ","
                                   << csrpRow[3] << ",No Match,-,-,-,No Match,-,-\n";
                        noMatches++;
                    }
                }
            }
        }
    }

    // Print the number of matches and non-matches
    std::cout << "Exact Matches: " << exactMatches << std::endl;
    std::cout << "Fuzz Matches: " << fuzzMatches << std::endl;
    std::cout << "No Matches: " << noMatches << std::endl;

    // Close the output file
    outputFile.close();

    return 0;
}

//Order for Matching
//1. Exact match of "company name" to "assignee"
//2. Exact match of "cleaned company name" to "cleaned assignee"
//3. Closest match of "company name" to "assignee"
//4. Closest match of "cleaned company name" to "cleaned assignee"
//5. No match found
//6. Write to CSV file
//7. Print number of matches and non-matches
//8. End program


//Compile: g++ -o CSRP_PERMCO_to_AllAssignee CSRP_PERMCO_to_AllAssignee.cpp
//Run: ./CSRP_PERMCO_to_AllAssignee