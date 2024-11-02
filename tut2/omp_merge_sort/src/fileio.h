#ifndef _FILEIO 
#define _FILEIO

int read_array_from_file(char* filename, int* array, int size);
int append_array_to_file(char* filename, int* array, int size);  
int write_array_to_file(char* filename, int* array, int size);  
  
#endif
