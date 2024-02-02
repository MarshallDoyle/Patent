import csv
import numpy as np
from functools import lru_cache

def to_lower(string):
    return string.lower()

def strip(string):
    return string.strip()

def compare_strings(str1, str2):
    return to_lower(strip(str1)) == to_lower(strip(str2))

@lru_cache(maxsize=None)
def levenshtein_distance(s1, s2):
    if not s1:
        return len(s2)
    if not s2:
        return len(s1)
    if s1[0] == s2[0]:
        return levenshtein_distance(s1[1:], s2[1:])
    return 1 + min(
        levenshtein_distance(s1, s2[1:]),  # Insertion
        levenshtein_distance(s1[1:], s2),  # Deletion
        levenshtein_distance(s1[1:], s2[1:])  # Substitution
    )

def binary_search(data, key, value):
    low = 0
    high = len(data) - 1
    while low <= high:
        mid = (low + high) // 2
        mid_value = to_lower(strip(data[mid][key]))
        if mid_value == to_lower(strip(value)):
            return mid
        elif mid_value < to_lower(strip(value)):
            low = mid + 1
        else:
            high = mid - 1
    return -1

def binary_search_with_levenshtein(data, key, value):
    low = 0
    high = len(data) - 1
    closest_match = -1
    closest_distance = np.inf
    while low <= high:
        mid = (low + high) // 2
        mid_value = to_lower(strip(data[mid][key]))
        search_value = to_lower(strip(value))
        if mid_value == search_value:
            return mid
        else:
            distance = levenshtein_distance(mid_value, search_value)
            if distance < closest_distance:
                closest_distance = distance
                closest_match = mid
            if mid_value < search_value:
                low = mid + 1
            else:
                high = mid - 1
    return closest_match

def length_score(str1, str2, levenshtein_score):
    delta_length = abs(len(str1) - len(str2))
    return delta_length * levenshtein_score

def read_csv(file_path, header=False):
    with open(file_path, newline='', encoding='utf-8') as csvfile:
        reader = csv.reader(csvfile)
        if header:
            next(reader, None)  # skip the headers
        return [row for row in reader]

def write_csv_append(filename, data):
    with open(filename, 'a', newline='') as file:  # 'a' mode for append
        writer = csv.writer(file)
        writer.writerows(data)

def process_batch(data, batch, matched_data, no_match_data):
    for i, assignee_row in enumerate(batch):
        assignee = assignee_row[1]  # Using Uncleaned Assignee for matching
        search_index = binary_search(data, 0, assignee)
        if search_index != -1:
            # Exact match found
            levenshtein_score = levenshtein_distance(to_lower(strip(assignee)), to_lower(strip(data[search_index][0])))
            length_score_val = length_score(assignee, data[search_index][0], levenshtein_score)
            row = [data[search_index][0], data[search_index][1], data[search_index][2], assignee, "Exact", str(levenshtein_score), str(length_score_val)]
            matched_data.append(row)
        else:
            # Closest match
            search_index = binary_search_with_levenshtein(data, 0, assignee)
            if search_index != -1:
                levenshtein_score = levenshtein_distance(to_lower(strip(assignee)), to_lower(strip(data[search_index][0])))
                length_score_val = length_score(assignee, data[search_index][0], levenshtein_score)
                row = [data[search_index][0], data[search_index][1], data[search_index][2], assignee, "No Match", str(levenshtein_score), str(length_score_val)]
                no_match_data.append(row)

def main():
    try:
        assignees_path = "C:/Users/marsh/Patent Tracking/Sorted/Sorted_unique_assignee/Unique_Assignee_sorted_by_Uncleaned.csv"
        data_path = "C:/Users/marsh/Patent Tracking/Sorted/Sorted_capital_cusip/Sorted_Cleaned_capital_cusip_assignee.csv"
        matched_output_path = "C:/Users/marsh/Patent Tracking/Matched/Matched_CUSIP.csv"
        unmatched_output_path = "C:/Users/marsh/Patent Tracking/Matched/UnMatched_CUSIP.csv"
        
        assignees = read_csv(assignees_path, header=True)
        data = read_csv(data_path, header=True)

        batch_size = 20000
        for start in range(0, len(assignees), batch_size):
            end = start + batch_size
            batch = assignees[start:end]

            matched_data = []
            no_match_data = []
            
            process_batch(data, batch, matched_data, no_match_data)

            # Append the batch results to CSV files
            write_csv_append(matched_output_path, matched_data)
            write_csv_append(unmatched_output_path, no_match_data)

            print(f"Finished processing rows {start + 1} to {end}")

        # Print the final number of matches and non-matches
        print(f"Finished Matching. Check {matched_output_path} and {unmatched_output_path} for results.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()