'''
This Program just takes the two csv files and matches the data only for the words that begin with "A"
This Program does not perform fuzzy matching, only exact for clean terms
This Program Returned 96 matches
'''

import csv
import time

def read_and_filter_csv(file_path, column_name):
    """Read a CSV file and filter rows based on the first letter of a specified column."""
    filtered_data = []
    with open(file_path, mode='r', newline='', encoding='utf-8') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if row[column_name].strip().lower().startswith('a'):
                filtered_data.append(row)
    return filtered_data

def write_output_data(file_path, fieldnames, data):
    """Write data to a CSV file."""
    with open(file_path, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(data)

def exact_match_and_combine(first_file, second_file, first_column, second_column):
    first_data = read_and_filter_csv(first_file, first_column)
    second_data = read_and_filter_csv(second_file, second_column)

    matched_data = []
    unmatched_data = []
    total_matches = len(first_data)
    matches_processed = 0

    for row in first_data:
        match_start_time = time.time()
        match_found = False
        for match_row in second_data:
            if row[first_column].strip().lower() == match_row[second_column].strip().lower():
                matched_data.append({**row, **match_row})
                match_found = True
                break
        match_end_time = time.time()
        matches_processed += 1
        percentage_done = (matches_processed / total_matches) * 100
        if match_found:
            print(f"Match found: {row[first_column]} with {match_row[second_column]}, {percentage_done:.2f}% complete. Match time: {match_end_time - match_start_time:.4f} seconds")
        else:
            unmatched_data.append(row)
            print(f"No match for: {row[first_column]}, {percentage_done:.2f}% complete. Match time: {match_end_time - match_start_time:.4f} seconds")

    return matched_data, unmatched_data

# Define file paths and column names
first_csv_file = 'e:\\Patent Tracking\\Sorted\\Sorted_unique_assignee\\Cleaned.csv'
second_csv_file = 'e:\\Patent Tracking\\Sorted\\Sorted_sec_cik\\Cleaned.csv'
first_csv_column = 'assignee'
second_csv_column = 'Cleaned_company_name'
output_matched_csv = 'output_matched.csv'
output_unmatched_csv = 'output_unmatched.csv'

# Perform exact match and combine data
matched, unmatched = exact_match_and_combine(first_csv_file, second_csv_file, first_csv_column, second_csv_column)

# Write matched and unmatched data to output files
write_output_data(output_matched_csv, list(matched[0].keys()) if matched else [], matched)
write_output_data(output_unmatched_csv, list(unmatched[0].keys()) if unmatched else [], unmatched)
