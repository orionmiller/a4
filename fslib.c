/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#include "fslib.h"

#include "partition.h"

block* getPartTable(FILE *fs, uint32_t offset, uint8_t p_table_num)
{

  block *Block;
  size_t read_size;
  uint8_t ptable_sig = 0;
  uint32_t ptable_pos;

 /*partition table position*/
  ptable_pos = offset + PART_TABLE_BASE + (p_table_num*P_TABLE_SIZE);

  FATALCALL((Block=(block *)malloc(P_TABLE_SIZE))==NULL,"partition: malloc");

  /*Check Partition Signature 1 - Magic Num Below*/
  FATALCALL(fseek(fs, offset + PART_SIG_1_OFF , SEEK_SET)==-1,"partition fseek");
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
  FATALCALL(fseek(fs, offset + PART_SIG_2_OFF , SEEK_SET)==-1,"partition fseek");
  read_size = fread((unsigned char *)&ptable_sig, 1, sizeof(unsigned char), fs);
  if (!correctRead(read_size, sizeof(unsigned char), fs))
    return NULL;
  if (ptable_sig != PART_SIG_2)
    {
      fprintf(stderr, "Incorrect Partition Signature at %lX\n", ftell(fs));
      fprintf(stderr, "Value: 0x%x\n", ptable_sig);
      return NULL;
    }

  /*Read in Partition Table*/
  FATALCALL(fseek(fs, ptable_pos, SEEK_SET)==-1,"partition fseek");
  read_size = fread((block *)Block, 1, P_TABLE_SIZE, fs); /*magic number*/
  if (!correctRead(read_size, P_TABLE_SIZE,fs))
    return NULL;
  

  /*Check Partition Table*/
  if (Block->p_table.type != MINIX_PART)
    {
      fprintf(stderr, "Not Minix Part - Type: (0x%x)\n", Block->p_table.type);
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

block * getSuperBlock(FILE *fs, uint32_t offset)
{
  block *Block;
  size_t read_size;

  FATALCALL((Block=(block *)malloc(S_BLOCK_SIZE))==NULL,"super block: malloc");

  /*Check Super Block - Magic Num Below*/
  FATALCALL(fseek(fs, offset + S_BLOCK_BASE, SEEK_SET)==-1,"partition fseek");
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

inode * getFile(FILE *fs, char ** path, uint32_t inode_off, uint32_t data_zone_off)
{
  
  /* grab root inode data   */
  char currstr[60];
  int inodenum = 0;/*magic number*/
  uint32_t j, i = 0;
  uint32_t numEntries; /*number of filename|inode # entries*/
  uint8_t * file_data;
  inode current_inode;
  uint32_t root_inode = inode_off + INODE_SIZE;
  size_t read_size;

  /*Read in root inode*/
  FATALCALL(fseek(fs, root_inode, SEEK_SET)==-1,"fseek");
  read_size = fread((inode *)&current_inode, 1, INODE_SIZE, fs); /*magic number*/
  if (!correctRead(read_size, INODE_SIZE, fs))
    return NULL;

  /*grab root inode data*/  

  FATALCALL((file_data = malloc( )),"malloc");
  numEntries =   /* numEntries = fiel size / 64;*/
  while(path[i]!=NULL)
    {
      j = 0;
      inodenum = 0;
      while(j<numEntries)
	{
	  memcpy(memarea+(FILE_ENTRY_SIZE*j),FILENAME_SIZE,currstr);
	  if(strncmp(FILENAME_SIZE,currstr,path[i])=0)
	    {
	      memcpy(memarea+(FILE_ENTRY_SIZE*j)+FILENAME_SIZE,INODE_NUM_SIZE,inodenum);
	      break;
	    }       
	}       
      
      if(inodenum==0)
	{
	  fprintf(stderr, "File Doesn't Exist\n");
	  return NULL;
	}       
    }       

  return 
}

uint8_t * getData(FILE *fs, uint8_t *data, inode *Inode, uint32_t data_zone_off, uint32_t zone_size)
{
  uint32_t filesize = Inode->size;
  uint32_t num_zones = filesize / zone_size;
  uint32_t current_zone;
  if (filesize%zone_size > 0)
    {
      num_zones++;
    }
  FATALCALL((data=malloc(filesize))==NULL, "malloc");
  for (current_zone = 0; current_zone < num_zones; current_zone++)
    {
      if (current_zone < 7)
	{

	}
      else if (current_zone < )

	}
    }
  return ;
}

void getDirectZone(uint8_t *file, uint32_t current_file_zone, uint32_t data_zone_off, uint32_t current_data_zone)
{
   
}

void getIndrZone(uint8_t *data, uint32_t current_zone, uint32_t *data_zone_off)
{

}

void getDblIndrZone(uint8_t *data, uint32_t current_zone, uint32_t *data_zone_off)
{

}
