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
            row_to_write = {fieldname: data_row.get(fieldname, '') for fieldname in fieldnames}
            writer.writerow(row_to_write)

def match_and_fuzzy_match_rows(csrp_file, assignee_file, output_file):
    csrp_data = read_csv(csrp_file)
    assignee_data = read_csv(assignee_file)
    assignee_data_by_year = {row['year']: [] for row in assignee_data}
    for row in assignee_data:
        assignee_data_by_year[row['year']].append(row)

    matched_data = []
    total = len(csrp_data)
    processed = 0
    start_time = time.time()

    for csrp_row in csrp_data:
        processed += 1
        assignee_year_data = assignee_data_by_year.get(csrp_row['year'], [])
        matches = []
        for assignee_row in assignee_year_data:
            if csrp_row['Cleaned_company_name'][0].lower() == assignee_row['Cleaned_assignee'][0].lower():
                fuzz_score = fuzz.ratio(csrp_row['Cleaned_company_name'].lower(), assignee_row['Cleaned_assignee'].lower())
                if fuzz_score >= 84:
                    matches.append((assignee_row, fuzz_score))

        # Sort matches by score in descending order and pick top 5
        matches = sorted(matches, key=lambda x: x[1], reverse=True)[:5]
        match_data = {}
        for i, match in enumerate(matches):
            match_data[f'Match{i+1}'] = match[0]['assignee']
            match_data[f'Score{i+1}'] = match[1]

        if matches:
            combined_row = {**csrp_row, **match_data}
            matched_data.append(combined_row)

        if processed % 10 == 0 or processed == total:  # Update progress every 10 items or at the last item
            print(f"Processed {processed}/{total} entries ({(processed/total)*100:.2f}%)")

    # Print total time for processing
    print(f"Matching completed in {time.time() - start_time:.2f} seconds.")

    # Write the results to the CSV
    write_csv(output_file, ['company_name', 'permco', 'year', 'Cleaned_company_name', 'Match1', 'Score1', 'Match2', 'Score2', 'Match3', 'Score3', 'Match4', 'Score4', 'Match5', 'Score5'], matched_data)

# File paths and execution
csrp_csv_file = 'E:/Patent Tracking/March/Sorted_by_cleaned_cleaned_csrp_permco.csv'
assignee_csv_file = 'E:/Patent Tracking/March/Sorted_by_cleaned_cleaned_assignee_all_unique.csv'
output_matched_csv = 'E:/Patent Tracking/March/SamplerOutput.csv'

match_and_fuzzy_match_rows(csrp_csv_file, assignee_csv_file, output_matched_csv)
