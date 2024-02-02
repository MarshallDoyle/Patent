import csv
import sys 

input_file_path = sys.argv[1]
output_file_path = sys.argv[2]
column_index_to_sort_by = int(sys.argv[3])

def sort_csv_by_column_index(input_file_path, output_file_path, column_index):
    try:
        with open(input_file_path, mode='r', newline='', encoding='utf-8') as file_in:
            reader = csv.DictReader(file_in)
            fieldnames = reader.fieldnames

            # Check if the specified column index is valid
            if column_index < 0 or column_index >= len(fieldnames):
                print(f"Column index {column_index} is out of range for {input_file_path}. Skipping this file.")
                return
            
            # Retrieve the column name using the index
            column_name = fieldnames[column_index]

            # Read data and sort by the specified column
            data = [row for row in reader]
            data.sort(key=lambda row: row.get(column_name, ''))

        with open(output_file_path, mode='w', newline='', encoding='utf-8') as file_out:
            writer = csv.DictWriter(file_out, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(data)

        print(f"File sorted and saved to {output_file_path}")

    except Exception as e:
        print(f"An error occurred: {e}")


sort_csv_by_column_index(input_file_path, output_file_path, column_index_to_sort_by)
