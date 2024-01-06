import csv

def sort_csv_alphabetically(input_file_path, output_file_path, column_name):
    try:
        with open(input_file_path, mode='r', newline='', encoding='utf-8') as file_in:
            reader = csv.DictReader(file_in)
            fieldnames = reader.fieldnames

            # Check if the specified column exists in the CSV
            if column_name not in fieldnames:
                print(f"Column '{column_name}' not found in {input_file_path}. Skipping this file.")
                return

            # Read data and sort by the specified column
            data = [row for row in reader]
            data.sort(key=lambda row: row.get(column_name, '').lower())

        with open(output_file_path, mode='w', newline='', encoding='utf-8') as file_out:
            writer = csv.DictWriter(file_out, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(data)

        print(f"File sorted and saved to {output_file_path}")

    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
input_file_path = 'path/to/input.csv' # Replace with your input file path
output_file_path = 'path/to/output.csv' # Replace with your desired output file path
column_to_sort_by = 'column_name' # Replace with the column you want to sort by

sort_csv_alphabetically(input_file_path, output_file_path, column_to_sort_by)
