/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#include "fslib.h"

#include "partition.h"

block* getPartTable(FILE *fs, uint8_t ptable_off)
{
  /*set offset to beginning of file
   *else check partition table
   *checkf for partition signatures
   *check for minix partition type
   *
   */

  block *Block;
  size_t read_size;
  uint8_t ptable_sig = 0;
  uint32_t ptable_pos;

  if (ptable_off < 0 || ptable_off > 3)
    {
      fprintf(stderr, "Partition # %u is invalid.\n", ptable_off);
      return NULL;
    }

  ptable_pos = ptable_off * P_TABLE_SIZE + PART_TABLE_BASE;

  FATALCALL((Block=(block *)malloc(P_TABLE_SIZE))==NULL,"partition: malloc");

  /*Check Partition Signature 1 - Magic Num Below*/
  FATALCALL(fseek(fs, PART_SIG_1_OFF , SEEK_SET)==-1,"partition fseek");
  read_size = fread((unsigned char *)&ptable_sig, 1, sizeof(unsigned char), fs);
  if (!correctRead(read_size, sizeof(unsigned char), fs))
    return NULL;
  if (ptable_sig != PART_SIG_1)
    {
      fprintf(stderr, "Incorrect Partition Signature at %lX\n", ftell(fs));
      fprintf(stderr, "Value: 0x%x\n", ptable_sig);
      return NULL;
    }

  /*Check Partition Signature 2 - Magic Num Below*/
  FATALCALL(fseek(fs, PART_SIG_2_OFF , SEEK_SET)==-1,"partition fseek");
  read_size = fread((unsigned char *)&ptable_sig, 1, sizeof(unsigned char), fs);
  if (!correctRead(read_size, sizeof(unsigned char), fs))
    return NULL;
  if (ptable_sig != PART_SIG_2)
    {
      fprintf(stderr, "Incorrect Partition Signature at %lX\n", ftell(fs));
      fprintf(stderr, "Value: 0x%x\n", ptable_sig);
      return NULL;
    }

  /*Read in Partition Table - Magic Num Below*/
  FATALCALL(fseek(fs, ptable_pos, SEEK_SET)==-1,"partition fseek");
  read_size = fread((block *)Block, 1, P_TABLE_SIZE, fs); /*magic number*/
  if (!correctRead(read_size, P_TABLE_SIZE,fs))
    return NULL;
  

  /*Check Partition Table*/

  if (Block->part.type != MINIX_PART)
    {
      fprintf(stderr, "Not Minix Part - Type: (0x%x)\n", Block->part.type);
      return NULL;
    }

  return Block;
}

/*try to rename */
int  correctRead(size_t read_size, size_t correct_size, FILE *fs)
{
  if (read_size != correct_size)
    {
      if (feof(fs))
	{
	  fprintf(stderr, "Disk is the wrong size.\n");
	}
      else if (ferror(fs))
	{
	  perror("getPartTable(): fread error");
	}
      else 
	{
	  fprintf(stderr, "Unknown Error (ferror & feof not set.)\n");
	}
      return 0;
    }
  return 1;
}

block * getSuperBlock(FILE *fs)
{
  block *Block;
  size_t read_size;

  FATALCALL((Block=(block *)malloc(S_BLOCK_SIZE))==NULL,"super block: malloc");

  /*Check Super Block - Magic Num Below*/
  FATALCALL(fseek(fs, S_BLOCK_OFF, SEEK_SET)==-1,"partition fseek");
  read_size = fread((block *)Block, 1, S_BLOCK_SIZE, fs); /*magic number*/
  if (!correctRead(read_size, S_BLOCK_SIZE, fs))
    return NULL;

  if(Block->s_block.magic != S_BLOCK_MAGIC_LITTLE)
    {
      fprintf(stderr, "Super Block - Bad Magic Number");
      fprintf(stderr, "(0x%.2X)\n",Block->s_block.magic);
      return NULL;
    }

  return Block;
}
