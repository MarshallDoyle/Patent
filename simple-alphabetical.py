import csv

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
            data.sort(key=lambda row: row.get(column_name, '').lower())

        with open(output_file_path, mode='w', newline='', encoding='utf-8') as file_out:
            writer = csv.DictWriter(file_out, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(data)

        print(f"File sorted and saved to {output_file_path}")

    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
input_file_path = 'c:/Users/marsh/Patent Tracking/Cleaned/Cleaned_csrp_permco.csv'
output_file_path = 'c:/Users/marsh/Patent Tracking/Sorted/Sorted_crsp_permo/CSRP_Permco_Sorted_by_UnCleaned.csv'
column_index_to_sort_by = 0 # Replace with the index of the column you want to sort by (starting from 0)

sort_csv_by_column_index(input_file_path, output_file_path, column_index_to_sort_by)
