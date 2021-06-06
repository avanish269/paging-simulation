/*
Avanish Kumar Singh
2017MT10728
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const int mask = 4294963200;

typedef struct page_table_entry
{
    int virtual_page_number, physical_frame_number, valid_bit, dirty_bit;
} pte;

typedef struct physical_frame_entry
{
    int physical_frame_number, virtual_page_number;
} pfe;

int count_lines(char *trace_file)
{
    int no_of_memory_accesses = 0;
    FILE *file = fopen(trace_file, "r");
    if(file == NULL)
    {
        printf("Error in opening file\n");
        exit(EXIT_FAILURE);
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, file);
    while(read != -1)
    {
        no_of_memory_accesses++;

        // printf("Line length: %zu\n", read);
        // printf("Line: %s", line);

        const char temp[2] = " ";
        char *virtual_memory_address = strtok(line, temp);
        char *token = strtok(NULL, temp);
        char access_type = token[0];
        // printf("%s %c\n", virtual_memory_address, access_type);
        read = getline(&line, &len, file);
    }
    fclose(file);
    return no_of_memory_accesses;
}

void print_verbose(int read_page, int old_page, int dirty_bit)
{
    if (dirty_bit == 1)
    {
        printf("Page 0x%05x was read from disk, page 0x%05x was written to the disk.\n", read_page, old_page);
    }
    else
    {
        printf("Page 0x%05x was read from disk, page 0x%05x was dropped (it was not dirty).\n", read_page, old_page);
    }
}

void print_results(int no_of_memory_accesses, int no_of_misses, int no_of_writes, int no_of_drops)
{
    printf("Number of memory accesses: %d\n", no_of_memory_accesses);
    printf("Number of misses: %d\n", no_of_misses);
    printf("Number of writes: %d\n", no_of_writes);
    printf("Number of drops: %d\n", no_of_drops);
}

void optimal_replacement_policy(pfe* physical_memory, pte *page_table, int no_of_frames, int no_of_pages, long int *list_of_memory_accesses, int *list_of_access_type, int no_of_memory_accesses, int verbose)
{
    printf("Optimal\n");
    int isFilled = 0, i = 0;
    int no_of_misses = 0, no_of_drops = 0, no_of_writes = 0;
    for(; i < no_of_memory_accesses; i++)
    {
        int accessed_virtual_page_number = (list_of_memory_accesses[i] & mask) >> 12;
        accessed_virtual_page_number--;
        int access_type = list_of_access_type[i];
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 0)
            continue;
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 1)
        {
            page_table[accessed_virtual_page_number].dirty_bit = 1;
            continue;
        }
        no_of_misses++;
        page_table[accessed_virtual_page_number].valid_bit = 1;
        if(access_type == 1)
            page_table[accessed_virtual_page_number].dirty_bit = 1;
        else
            page_table[accessed_virtual_page_number].dirty_bit = 0;
        if(isFilled != no_of_frames)
        {
            page_table[accessed_virtual_page_number].physical_frame_number = isFilled;
            physical_memory[isFilled].virtual_page_number = accessed_virtual_page_number;
            isFilled++;
        }
        else
        {

        }
    }
    // printf("%d\n", isFilled);
    // for(int i = 0; i < no_of_pages; i++)
    // {
    //     printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
    // }
    //
    // for(int i = 0; i < no_of_frames; i++)
    // {
    //     printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
    // }
}

void fifo_policy(pfe* physical_memory, pte *page_table, int no_of_frames, int no_of_pages, long int *list_of_memory_accesses, int *list_of_access_type, int no_of_memory_accesses, int verbose)
{
    printf("FIFO\n");
    int isFilled = 0, i = 0;
    int no_of_misses = 0, no_of_drops = 0, no_of_writes = 0;
    for(; i < no_of_memory_accesses; i++)
    {
        int accessed_virtual_page_number = (list_of_memory_accesses[i] & mask) >> 12;
        accessed_virtual_page_number--;
        int access_type = list_of_access_type[i];
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 0)
            continue;
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 1)
        {
            page_table[accessed_virtual_page_number].dirty_bit = 1;
            continue;
        }
        no_of_misses++;
        page_table[accessed_virtual_page_number].valid_bit = 1;
        if(access_type == 1)
            page_table[accessed_virtual_page_number].dirty_bit = 1;
        else
            page_table[accessed_virtual_page_number].dirty_bit = 0;
        if(isFilled != no_of_frames)
        {
            page_table[accessed_virtual_page_number].physical_frame_number = isFilled;
            physical_memory[isFilled].virtual_page_number = accessed_virtual_page_number;
            isFilled++;
        }
        else
        {

        }
    }
    // printf("%d\n", isFilled);
    // for(int i = 0; i < no_of_pages; i++)
    // {
    //     printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
    // }
    //
    // for(int i = 0; i < no_of_frames; i++)
    // {
    //     printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
    // }
}

void random_policy(pfe* physical_memory, pte *page_table, int no_of_frames, int no_of_pages, long int *list_of_memory_accesses, int *list_of_access_type, int no_of_memory_accesses, int verbose)
{
    printf("Random\n");
    int isFilled = 0, i = 0;
    int no_of_misses = 0, no_of_drops = 0, no_of_writes = 0;
    for(; i < no_of_memory_accesses; i++)
    {
        int accessed_virtual_page_number = (list_of_memory_accesses[i] & mask) >> 12;
        accessed_virtual_page_number--;
        int access_type = list_of_access_type[i];
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 0)
            continue;
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 1)
        {
            page_table[accessed_virtual_page_number].dirty_bit = 1;
            continue;
        }
        no_of_misses++;
        page_table[accessed_virtual_page_number].valid_bit = 1;
        if(access_type == 1)
            page_table[accessed_virtual_page_number].dirty_bit = 1;
        else
            page_table[accessed_virtual_page_number].dirty_bit = 0;
        if(isFilled != no_of_frames)
        {
            page_table[accessed_virtual_page_number].physical_frame_number = isFilled;
            physical_memory[isFilled].virtual_page_number = accessed_virtual_page_number;
            isFilled++;
        }
        else
        {

        }
    }
    // printf("%d\n", isFilled);
    // for(int i = 0; i < no_of_pages; i++)
    // {
    //     printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
    // }
    //
    // for(int i = 0; i < no_of_frames; i++)
    // {
    //     printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
    // }
}

void lru_policy(pfe* physical_memory, pte *page_table, int no_of_frames, int no_of_pages, long int *list_of_memory_accesses, int *list_of_access_type, int no_of_memory_accesses, int verbose)
{
    printf("LRU\n");
    int isFilled = 0, i = 0;
    int no_of_misses = 0, no_of_drops = 0, no_of_writes = 0;
    for(; i < no_of_memory_accesses; i++)
    {
        int accessed_virtual_page_number = (list_of_memory_accesses[i] & mask) >> 12;
        accessed_virtual_page_number--;
        int access_type = list_of_access_type[i];
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 0)
            continue;
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 1)
        {
            page_table[accessed_virtual_page_number].dirty_bit = 1;
            continue;
        }
        no_of_misses++;
        page_table[accessed_virtual_page_number].valid_bit = 1;
        if(access_type == 1)
            page_table[accessed_virtual_page_number].dirty_bit = 1;
        else
            page_table[accessed_virtual_page_number].dirty_bit = 0;
        if(isFilled != no_of_frames)
        {
            page_table[accessed_virtual_page_number].physical_frame_number = isFilled;
            physical_memory[isFilled].virtual_page_number = accessed_virtual_page_number;
            isFilled++;
        }
        else
        {

        }
    }
    // printf("%d\n", isFilled);
    // for(int i = 0; i < no_of_pages; i++)
    // {
    //     printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
    // }
    //
    // for(int i = 0; i < no_of_frames; i++)
    // {
    //     printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
    // }
}

void clock_policy(pfe* physical_memory, pte *page_table, int no_of_frames, int no_of_pages, long int *list_of_memory_accesses, int *list_of_access_type, int no_of_memory_accesses, int verbose)
{
    printf("Clock\n");
    int isFilled = 0, i = 0;
    int no_of_misses = 0, no_of_drops = 0, no_of_writes = 0;
    for(; i < no_of_memory_accesses; i++)
    {
        int accessed_virtual_page_number = (list_of_memory_accesses[i] & mask) >> 12;
        accessed_virtual_page_number--;
        int access_type = list_of_access_type[i];
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 0)
            continue;
        if(page_table[accessed_virtual_page_number].valid_bit == 1 && access_type == 1)
        {
            page_table[accessed_virtual_page_number].dirty_bit = 1;
            continue;
        }
        no_of_misses++;
        page_table[accessed_virtual_page_number].valid_bit = 1;
        if(access_type == 1)
            page_table[accessed_virtual_page_number].dirty_bit = 1;
        else
            page_table[accessed_virtual_page_number].dirty_bit = 0;
        if(isFilled != no_of_frames)
        {
            page_table[accessed_virtual_page_number].physical_frame_number = isFilled;
            physical_memory[isFilled].virtual_page_number = accessed_virtual_page_number;
            isFilled++;
        }
        else
        {

        }
    }
    // printf("%d\n", isFilled);
    // for(int i = 0; i < no_of_pages; i++)
    // {
    //     printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
    // }
    //
    // for(int i = 0; i < no_of_frames; i++)
    // {
    //     printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
    // }
}

int main(int argc, char *argv[])
{
    clock_t tic = clock();
    int no_of_memory_accesses = 0;

    // printf("Argument count: %d\nArgument Vector: \n", argc);
    // for(int i = 0; i < argc; i++)
    // {
    //     printf("%d. %s\n", (i + 1), argv[i]);
    // }

    //Converting system arguments into useful data
    char *program_name = argv[0];
    char *trace_file = argv[1];
    char *no_of_frames_str = argv[2];
    int no_of_frames = atoi(argv[2]);
    char *method = argv[3];

    int verbose = 0;
    if(argc == 5)
    {
        if(strcmp(argv[4], "-verbose") == 0)
        {
            verbose = 1;
        }
        else
        {
            printf("To use Verbose mode use -verbose at the end\n");
            exit(EXIT_FAILURE);
        }
    }

    // printf("Argument Count: %d\n", argc);
    // printf("Program Name: %s\n", program_name);
    // printf("Trace File: %s\n", trace_file);
    // printf("Number of Frames: %d\n", no_of_frames);
    // printf("Strategy: %s\n", method);
    // printf("Verbose: %d\n", verbose);

    no_of_memory_accesses = count_lines(trace_file);
    // printf("Memory Access: %d\n", no_of_memory_accesses);

    long int *list_of_memory_accesses = malloc(no_of_memory_accesses * sizeof(long int));
    int *list_of_access_type = malloc(no_of_memory_accesses * sizeof(int));
    int idx = 0;
    FILE *file = fopen(trace_file, "r");
    if(file == NULL)
    {
        printf("Error in opening file\n");
        exit(EXIT_FAILURE);
    }
    int max_virtual_memory_address = -1;
    char *line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, file);
    while(read != -1)
    {
        // printf("Line length: %zu\n", read);
        // printf("Line: %s", line);
        const char temp[2] = " ";
        char *virtual_memory_address = strtok(line, temp);
        char *token = strtok(NULL, temp);
        char access_type = token[0];
        if(access_type == 'W')
        {
            list_of_access_type[idx] = 1;
        }
        else
        {
            list_of_access_type[idx] = 0;
        }
        char *ptr;
        list_of_memory_accesses[idx] = strtol(virtual_memory_address, &ptr, 16);

        max_virtual_memory_address = max_virtual_memory_address > list_of_memory_accesses[idx] ? max_virtual_memory_address : list_of_memory_accesses[idx];

        read = getline(&line, &len, file);
        idx++;
    }
    fclose(file);
    // for(int i = 0; i < no_of_memory_accesses; i++)
    // {
    //     printf("%ld %d\n", list_of_memory_accesses[i], list_of_access_type[i]);
    // }

    int no_of_pages = (max_virtual_memory_address & mask) >> 12;
    // printf("Max virtual memory address: %d Max virtual page number: %d\n", max_virtual_memory_address, no_of_pages);
    pte *page_table = malloc(no_of_pages * sizeof(pte));
    pfe *physical_memory = malloc(no_of_frames * sizeof(pfe));
    memset(page_table, -1, no_of_pages * sizeof(pte));
    memset(physical_memory, -1, no_of_frames * sizeof(pfe));
    for(int i = 0; i < no_of_pages; i++)
    {
        // printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
        page_table[i].virtual_page_number = i;
        page_table[i].physical_frame_number = -1;
        page_table[i].valid_bit = 0;
        page_table[i].dirty_bit = 0;
        // printf("%d %d %d %d\n", page_table[i].virtual_page_number, page_table[i].physical_frame_number, page_table[i].valid_bit, page_table[i].dirty_bit);
    }

    for(int i = 0; i < no_of_frames; i++)
    {
        // printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
        physical_memory[i].physical_frame_number = i;
        // printf("%d %d\n", physical_memory[i].physical_frame_number, physical_memory[i].virtual_page_number);
    }

    if(strcmp(method, "OPT") == 0)
    {
        optimal_replacement_policy(physical_memory, page_table, no_of_frames, no_of_pages, list_of_memory_accesses, list_of_access_type, no_of_memory_accesses, verbose);
    }
    else if(strcmp(method, "FIFO") == 0)
    {
        fifo_policy(physical_memory, page_table, no_of_frames, no_of_pages, list_of_memory_accesses, list_of_access_type, no_of_memory_accesses, verbose);
    }
    else if(strcmp(method, "RANDOM") == 0)
    {
        random_policy(physical_memory, page_table, no_of_frames, no_of_pages, list_of_memory_accesses, list_of_access_type, no_of_memory_accesses, verbose);
    }
    else if(strcmp(method, "LRU") == 0)
    {
        lru_policy(physical_memory, page_table, no_of_frames, no_of_pages, list_of_memory_accesses, list_of_access_type, no_of_memory_accesses, verbose);
    }
    else if(strcmp(method, "CLOCK") == 0)
    {
        clock_policy(physical_memory, page_table, no_of_frames, no_of_pages, list_of_memory_accesses, list_of_access_type, no_of_memory_accesses, verbose);
    }
    else
    {
        printf("Wrong Method Entered\n");
        exit(EXIT_FAILURE);
    }

    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    exit(EXIT_SUCCESS);
}
