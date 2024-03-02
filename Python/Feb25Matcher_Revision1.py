import csv
import time

def read_and_filter_csv(file_path, column_name):
    """Read a CSV file and filter rows based on the first letter of a specified column."""
    filtered_data = []
    with open(file_path, mode='r', newline='', encoding='utf-8') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if row[column_name].strip().lower().startswith('a'):  # Normalize here for filtering
                filtered_data.append(row)
    return filtered_data

def write_output_data(file_path, fieldnames, data):
    """Write data to a CSV file."""
    with open(file_path, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        for data_row in data:
            writer.writerow(data_row)

def exact_match_and_combine(first_file, second_file, first_column, second_column):
    start_time = time.time()
    first_data = read_and_filter_csv(first_file, first_column)
    second_data = read_and_filter_csv(second_file, second_column)

    matched_data = []
    unmatched_data = []

    total = len(second_data)
    processed = 0

    for match_row in second_data:
        match_found = False
        for row in first_data:
            # Normalize both strings for comparison
            if match_row[second_column].strip().lower() == row[first_column].strip().lower():
                matched_data.append({**row, **match_row})
                match_found = True
                break

        if not match_found:
            unmatched_data.append(match_row)

        processed += 1
        if processed % 10 == 0 or processed == total:  # Update progress every 10 items or last item
            print(f"Processed {processed}/{total} entries ({(processed/total)*100:.2f}%)")

    print(f"Matching completed in {time.time() - start_time:.2f} seconds.")
    return matched_data, unmatched_data

# Define file paths and column names
first_csv_file = 'e:\\Patent Tracking\\Sorted\\Sorted_unique_assignee\\Cleaned.csv'
second_csv_file = 'e:\\Patent Tracking\\Sorted\\Sorted_sec_cik\\Cleaned.csv'
first_csv_column = 'assignee_cleaned'
second_csv_column = 'Cleaned_company_name'
output_matched_csv = 'output_matched.csv'
output_unmatched_csv = 'output_unmatched.csv'

# Perform exact match and combine data
matched, unmatched = exact_match_and_combine(first_csv_file, second_csv_file, first_csv_column, second_csv_column)

# Print summary
print(f"Total matches found: {len(matched)}")
print(f"Total unmatched: {len(unmatched)}")

# Write matched and unmatched data to output files
write_output_data(output_matched_csv, list(matched[0].keys()) if matched else [], matched)
write_output_data(output_unmatched_csv, list(unmatched[0].keys()) if unmatched else [], unmatched)
