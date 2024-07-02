#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct page_table_entry
{
  int page_number;
  int referenced;
  int next;
};

int page_in_memory(int page, struct page_table_entry *frames, int frames_in_memory)
{
  for (int i = 0; i < frames_in_memory; i++)
  {
    if (frames[i].page_number == page)
    {
      return i;
    }
  }

  return -1;
}

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
  struct page_table_entry *page_table_entries = malloc(sizeof(struct page_table_entry) * reference_string_length);

  // initalize the array of page table entires
  for (int i = 0; i < reference_string_length; i++)
  {
    page_table_entries[i].page_number = -1;
    page_table_entries[i].referenced = 0;
    page_table_entries[i].next = -1;
  }

  // read each token of the refrence string, and add it to the array
  int page_index = 0;
  char *token = strtok(reference_string, " ");
  while (token != NULL && page_index < reference_string_length)
  {
    int page = atoi(token);
    page_table_entries[page_index].page_number = page;

    // search back through the array for the closest matching page number. If a match is found, set the next value to this page index
    for (int i = page_index - 1; i >= 0; i--)
    {
      if (page_table_entries[i].page_number == page)
      {
        page_table_entries[i].next = page_index;
        break;
      }
    }

    token = strtok(NULL, " ");
    page_index++;
  }

  // Optimal replacement program
  int full_frames = 0;
  int page_faults = 0;
  struct page_table_entry *frames = malloc(sizeof(struct page_table_entry) * frames_in_memory);
  for (int i = 0; i < reference_string_length; i++)
  {
    // Check if page is in memory, if it continue
    int mem_location = page_in_memory(page_table_entries[i].page_number, frames, full_frames);
    if (mem_location > -1)
    {
      frames[mem_location] = page_table_entries[i];
      continue;
    }

    page_faults++;

    // if there are empty frames in memory add in the page without replacement
    if (full_frames < frames_in_memory)
    {
      frames[full_frames] = page_table_entries[i];
      full_frames++;
    }
    else // Find the frame with the highest future reference and replace it
    {
      int maxPageIndex = 0;
      for (int j = 0; j < frames_in_memory; j++)
      {
        // If the next value == -1 the page will not be used again
        if (frames[j].next == -1)
        {
          maxPageIndex = j;
          break;
        }
        else if (frames[j].next > frames[maxPageIndex].next)
        {
          maxPageIndex = j;
        }
      }
      frames[maxPageIndex] = page_table_entries[i];
    }
  }

  printf("\nOptimal page faults: %f\n", (float)page_faults / reference_string_length);

  // FIFO
  int frame_pointer = 0;
  full_frames = 0;
  page_faults = 0;
  for (int i = 0; i < reference_string_length; i++)
  {
    int mem_location = page_in_memory(page_table_entries[i].page_number, frames, full_frames);
    if (mem_location > -1)
    {
      continue;
    }

    page_faults++;

    if (full_frames < frames_in_memory)
    {
      frames[full_frames] = page_table_entries[i];
      full_frames++;
    }
    else
    {
      frames[frame_pointer] = page_table_entries[i];
      frame_pointer = (frame_pointer + 1) % frames_in_memory;
    }
  }

  printf("FIFO page faults: %f\n", (float)page_faults / reference_string_length);

  // Second Chance
  frame_pointer = 0;
  full_frames = 0;
  page_faults = 0;
  for (int i = 0; i < reference_string_length; i++)
  {
    int mem_location = page_in_memory(page_table_entries[i].page_number, frames, full_frames);
    if (mem_location > -1)
    {
      frames[mem_location].referenced = 1;
      continue;
    }

    page_faults++;

    if (full_frames < frames_in_memory)
    {
      frames[full_frames] = page_table_entries[i];
      frames[full_frames].referenced = 1;
      full_frames++;
    }
    else
    {
      while (1)
      {
        if (frames[frame_pointer].referenced == 1)
        {
          frames[frame_pointer].referenced = 0;
          frame_pointer = (frame_pointer + 1) % frames_in_memory;
        }
        else
        {
          frames[frame_pointer] = page_table_entries[i];
          frames[frame_pointer].referenced = 1;
          frame_pointer = (frame_pointer + 1) % frames_in_memory;
          break;
        }
      }
    }
  }

  printf("Second-Chance page faults: %f\n", (float)page_faults / reference_string_length);

  free(reference_string);
  free(page_table_entries);
  return 0;
}
