#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct page_table_entry
{
  int page_number;
  int referenced;
  int present;
  int next;
};

int main()
{
  int frames_in_memory;
  int num_of_pages;
  int reference_string_length;
  char *reference_string = NULL;

  // Read in the user input
  char *input = NULL;
  size_t len = 0;
  printf("Enter the number of frames in main memory: ");
  getline(&input, &len, stdin);
  sscanf(input, "%d", &frames_in_memory);
  free(input);

  input = NULL;
  len = 0;

  printf("Enter the number of pages in the page table: ");
  getline(&input, &len, stdin);
  sscanf(input, "%d", &num_of_pages);
  free(input);

  input = NULL;
  len = 0;

  printf("Enter the length of the reference string: ");
  getline(&input, &len, stdin);
  sscanf(input, "%d", &reference_string_length);
  free(input);

  input = NULL;
  len = 0;

  printf("Enter the reference string: ");
  getline(&reference_string, &len, stdin);

  // Create the array to store the page table entries.
  struct page_table_entry *page_table_entires = malloc(sizeof(struct page_table_entry) * reference_string_length);

  // initalize the array of page table entires
  for (int i = 0; i < num_of_pages; i++)
  {
    page_table_entires[i].page_number = -1;
    page_table_entires[i].present = 0;
    page_table_entires[i].referenced = 0;
    page_table_entires[i].next = -1;
  }

  // read each token of the refrence string, and add it to the array
  int page_index = 0;
  char *token = strtok(reference_string, " ");
  while (token != NULL && page_index < reference_string_length)
  {
    int page = atoi(token);
    page_table_entires[page_index].page_number = page;

    // search back through the array for the closest matching page number. If a match is found, set the next value to this page index
    for (int i = page_index - 1; i >= 0; i--)
    {
      if (page_table_entires[i].page_number == page)
      {
        page_table_entires[i].next = page_index;
        break;
      }
    }

    token = strtok(NULL, " ");
    page_index++;
  }





  

  free(reference_string);
  free(page_table_entires);
  return 0;
}
