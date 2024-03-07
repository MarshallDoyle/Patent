#!/bin/bash
#SBATCH -J AlphabeticalSorter
#SBATCH -p general
#SBATCH -o python%j.txt
#SBATCH -e python_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=8:00:00
#SBATCH --mail-user=gklinge@iu.edu
#SBATCH --mail-type=BEGIN,END
#SBATCH --mem 100G
#SBATCH -A r00638
#SBATCH --array=1-12

module load python

python -m venv ~/myenv
source ~/myenv/bin/activate

#Remember that the column number is 0-indexed, so column A is 0, B is 1, etc.

# Define file paths and column numbers for each job in the array
case "$SLURM_ARRAY_TASK_ID" in
    1)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_assignee_all.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_cleaned_cleaned_assignee_all.csv" #Give the file a name like "sorted_cleaned_assignee_all_by_assignee.csv"
        COLUMN_TO_READ=4  
        ;;
    2)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_assignee_all_unique.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_cleaned_cleaned_assignee_all_unique.csv"
        COLUMN_TO_READ=2 #This is the column that gets sorted
        ;;
    3)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_capitalcusip.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_cleaned_cleaned_capitalcusip.csv" 
        COLUMN_TO_READ=2  
        ;;
    4)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_csrp_permco.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_cleaned_cleaned_csrp_permco.csv"
        COLUMN_TO_READ=3
        ;;
    5)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_gvkey.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_cleaned_cleaned_gvkey.csv" 
        COLUMN_TO_READ=2
        ;;
    6)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_SEC_CIK.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_cleaned_cleaned_SEC_CIK.csv"
        COLUMN_TO_READ=2
        ;;
    7)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_assignee_all.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_uncleaned_cleaned_assignee_all.csv" #Give the file a name like "sorted_cleaned_assignee_all_by_assignee.csv"
        COLUMN_TO_READ=1  
        ;;
    8)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_assignee_all_unique.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_uncleaned_cleaned_assignee_all_unique.csv"
        COLUMN_TO_READ=0 #This is the column that gets sorted
        ;;
    9)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_capitalcusip.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_uncleaned_cleaned_capitalcusip.csv" 
        COLUMN_TO_READ=1  
        ;;
    10)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_csrp_permco.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_uncleaned_cleaned_csrp_permco.csv"
        COLUMN_TO_READ=0
        ;;
    11)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_gvkey.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_uncleaned_cleaned_gvkey.csv" 
        COLUMN_TO_READ=1
        ;;
    12)
        INPUT_FILE="/N/u/gklinge/Quartz/cleaned_SEC_CIK.csv"
        OUTPUT_FILE="/N/u/gklinge/Quartz/Sorted_by_uncleaned_cleaned_SEC_CIK.csv"
        COLUMN_TO_READ=1
        ;;
esac

cd $SLURM_SUBMIT_DIR

# Run the Python program with the specified arguments
srun python simple-alphabetical.py "$INPUT_FILE" "$OUTPUT_FILE" "$COLUMN_TO_READ" 
