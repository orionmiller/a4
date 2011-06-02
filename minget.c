#include <stdio.h>
#include <stdlib.h>
#include "fslib.h"
#include "debug.h"


int main(int argc, char *argv[])
{
  block *Block;
  FILE *fs;
  
  FATALCALL((fs = fopen(argv[1], "rb"))==NULL,"fopen");
  if((Block = getPartTable(fs))==NULL)
    {
      fprintf(stderr, "Partition Table Error\n");
    }
  else
    {
      printf("Print Partition\n");
      printf("Parition Type: %x", Block->part.type);
    }
  return EXIT_SUCCESS;
}
