/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#include "fslib.h"
#include <unistd.h>
#include "partition.h"

block* getPartTable(FILE *fs)
{
  /*set offset to beginning of file
   *read in 512 bytes
   *IF not 512 bytes or error report error & exit
   *else check partition table
   *checkf for partition signatures
   *check for minix partition type
   *
   */

  block *PartBlock;
  size_t read_size;
  FATALCALL((PartBlock=(block *)malloc(sizeof(struct partition)))==NULL,"partition: malloc");
  printf("Getting/Checking Partition\n");
  FATALCALL(fseek(fs, PART_TABLE_OFF , SEEK_SET)==-1,"partition fseek");
  printf("Seeked file\n");
  printf("Struct Partition Size: %lo\n", sizeof(struct partition));
  read_size = fread((block *)PartBlock, sizeof(struct partition), sizeof(unsigned char), fs);
  printf("Read file | read_size: %lo\n", read_size);
  if (read_size != sizeof(struct partition))
    {
      if (feof(fs))
	{
	  fprintf(stderr, "Not enough space for partition table and or a disk image\n");
	}
      else if (ferror(fs))
	{
	  perror("partition: fread error");
	}
      else 
	{
	  fprintf(stderr, "Something terrible happened (ferror & feof not set.\n");
	}
      return NULL;
    }
  return PartBlock;
}
