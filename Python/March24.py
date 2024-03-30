from rapidfuzz import fuzz
import csv
import time

def read_csv(file_path):
    """Read a CSV file and return the data as a list of dictionaries."""
    with open(file_path, mode='r', newline='', encoding='utf-8') as file:
        return list(csv.DictReader(file))

def write_csv(file_path, fieldnames, data):
    """Write data to a CSV file."""
    with open(file_path, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        for data_row in data:
            # Map data_row keys to the correct case as needed
            row_to_write = {fieldname: data_row.get(fieldname) for fieldname in fieldnames}
            writer.writerow(row_to_write)

def match_and_fuzzy_match_rows(csrp_file, assignee_file, output_file):
    csrp_data = read_csv(csrp_file)
    assignee_data = read_csv(assignee_file)

    # Index assignee file by year for faster lookup
    assignee_data_by_year = {}
    for row in assignee_data:
        assignee_data_by_year.setdefault(row['year'], []).append(row)

    matched_data = []
    total = len(csrp_data)
    processed = 0
    start_time = time.time()

    for csrp_row in csrp_data:
        processed += 1
        assignee_year_data = assignee_data_by_year.get(csrp_row['year'], [])
        # Exact match by 'company_name' to 'assignee' and 'year'
        exact_matches = [a_row for a_row in assignee_year_data if a_row['assignee'] == csrp_row['company_name']]

        # Process exact matches
        for assignee_row in exact_matches:
            # Check the first letter of 'Cleaned' fields only if 'company_name' to 'assignee' is a match
            if csrp_row['Cleaned_company_name'][0].lower() == assignee_row['Cleaned_assignee'][0].lower():
                matched_row = {**csrp_row, **assignee_row, 'Score': 100}  # Exact match score is 100
                matched_data.append(matched_row)

        # If no exact matches found, proceed to fuzzy matching
        if not exact_matches:
            best_fuzz_score = 0
            best_fuzz_match = None
            for assignee_row in assignee_year_data:
                # Check the first letter of 'Cleaned' fields before fuzzy matching
                if csrp_row['Cleaned_company_name'][0].lower() == assignee_row['Cleaned_assignee'][0].lower():
                    fuzz_score = fuzz.ratio(csrp_row['Cleaned_company_name'].lower(), assignee_row['Cleaned_assignee'].lower())
                    # Loop from a score of 99 down to 84 to find the best match
                    if 84 <= fuzz_score < 99 and fuzz_score > best_fuzz_score:
                        best_fuzz_score = fuzz_score
                        best_fuzz_match = assignee_row

            # If a fuzzy match is found with the best score
            if best_fuzz_match:
                matched_row = {**csrp_row, **best_fuzz_match, 'Score': best_fuzz_score}
                matched_data.append(matched_row)

        # Print progress
        if processed % 10 == 0 or processed == total:  # Update progress every 10 items or at the last item
            print(f"Processed {processed}/{total} entries ({(processed/total)*100:.2f}%)")

    # Print total time for processing
    print(f"Matching completed in {time.time() - start_time:.2f} seconds.")

    # Return only matched data
    return matched_data

# File paths based on the user's structure
csrp_csv_file = 'E:/Patent Tracking/March/Sorted_by_cleaned_cleaned_csrp_permco.csv'
assignee_csv_file = 'E:/Patent Tracking/March/Sorted_by_cleaned_cleaned_assignee_all_unique.csv'
output_matched_csv = 'E:/Patent Tracking/March/matched_output_March.csv'

# Perform matching
matched_rows = match_and_fuzzy_match_rows(csrp_csv_file, assignee_csv_file, output_matched_csv)

# Prepare fieldnames for the output CSV, matching case sensitivity from the matched data
output_fieldnames = [
    'company_name', 'permco', 'year', 'Cleaned_company_name',
    'assignee', 'Cleaned_assignee', 'Score'
]

write_csv(output_matched_csv, output_fieldnames, matched_rows)

# Print total matches found
print(f"Total matches found: {len(matched_rows)}")
