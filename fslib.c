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

inode * getFile(FILE *fs, char ** path, uint32_t inode_off, uint32_t part_off, uint32_t zone_size)
{
  uint32_t inode_num = ROOT_INODE_NUM;
  uint8_t * dir_data;
  inode * Inode;
  FATALCALL((Inode=malloc(INODE_SIZE))==NULL,"malloc");

  getInode(Inode, ROOT_INODE_NUM, inode_off, fs);

  if(path[0] == NULL)/*magic number*/
    return Inode;

  for(i=0; path[i] != NULL; i++)
    {
      getData(fs, dir_data, Inode, part_off, zone_size);
      inode_num = existsInPath(dir_data, path[i]);      
      if (inode_num == 0)
	return NULL;

      getInode(Inode, inode_num, inode_off, fs);

      if (path[i+1] == NULL)
	return Inode;

      free(dir_data);
    }
  return Inode;
}

void getInode(inode * Inode, uint32_t inode_num, uint32_t inode_off, FILE *fs)
{
  uint32_t file_pos = inode_off + (INODE_SIZE * (inode_num -1));
  FATALCALL(fseek(fs, file_pos, SEEK_SET)==-1,"fseek");
  FATALCALL((fread((inode *)Inode, 1, INODE_SIZE, fs))==NULL,"fread");/*magic num*/
}

uint32_t existsInPath(uint8_t * dir_data, uint32_t dir_size, uint8_t *filename)
{
  uint8_t dir_entry_name[60];
  uint32_t inode_num;
  uint8_t *dir_entry_pos = dir_data;

  while((dir_entry_pos - dir_data) < dir_size)
    {
      memcpy(dir_entry_pos,dir_entry_name,FILE_NAME_SIZE);
      if(strncmp(dir_entry_name,filename,FILE_NAME_SIZE)==0)
	{
	  memcpy(dir_entry_pos+FILE_NAME_SIZE, inode_num, INODE_NUM_SIZE);
	  return inode_num;
	}
      dir_entry_pos += DIR_ENTRY_SIZE;
    }
  return 0;
}

void getData(FILE *fs, uint8_t *data, inode *Inode, uint32_t part_off, uint32_t zone_size)
{
  uint32_t filesize = Inode->size;
  uint32_t num_zones = filesize / zone_size;
  uint32_t current_zone = 0;
  uint32_t read;
  if (filesize%zone_size > 0)
    {
      num_zones++;
    }
  FATALCALL((data=malloc(filesize))==NULL, "malloc");

  for(read=0; current_zone < num_zone && read<9; read++)
    {
      if(read<7)
	{
	  getDirectZone(data, Inode->zone[read], part_off, current_zone, zone_size, fs);
	}
      else if (read == 7)
	{
	  getIndrZone(data, Inode->ind_zone, part_off, current_zone, zone_size, fs);
	}
      else
	{
	  getDblZone(data, Inode->dbl_ind_zone, part_off, current_zone, zone_size, fs);
	}
    }
} 




/*
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

*/


void getDirectZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs)
{
  uint32_t file_pos = part_off + (zone_ptr * zone_size);
  uint8_t * data_pos = data + (*current_zone * zone_size); /*make sure correct add*/
  FATALCALL(fseek(fs, file_pos, SEEK_SET)==-1,"fseek");
  FATALCALL((fread((uint8_t *)data_pos, 1, zone_size, fs))==NULL,"fread");/*magic num*/
  (*current_zone) += 1;
}

void getIndrZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs)
{
  uint32_t read;
  uint32_t *zone_ptr_block;
  uint32_t dummy_curr_zone = 0;
  FATALCALL((zone_ptr_block=(uint32_t*)malloc(zone_size))==NULL, "malloc");
  /*may cause problems double check*/
  getDirectZone(zone_ptr_block, zone_ptr, part_off, &dummy_curr_zone, zone_size, fs);
  
  for(read=0; current_zone < num_zone && read < zone_size/; read++)
    {
      getDirectZone(data, zone_ptr_block[read], part_off, current_zone, zone_size, fs);
    }
  free(zone_ptr_block);
}

void getDblZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs)
{
  uint32_t read;
  uint32_t *indr_zone_ptr_block;
  uint32_t dummy_curr_zone = 0;
  FATALCALL((indr_zone_ptr_block=(uint32_t*)malloc(zone_size))==NULL, "malloc");
  /*may cause problems double check*/
  getDirectZone(indr_zone_ptr_block, zone_ptr, part_off, &dummy_curr_zone, zone_size, fs);
  
  for(read=0; current_zone < num_zone && read < zone_size/; read++)
    {
      getDirectZone(data, indr_zone_ptr_block[read], part_off, current_zone, zone_size, fs);
    }
  free(indr_zone_ptr_block);
}

/*							       
void getIndrZone(uint8_t *file, uint32_t current_file_zone, uint32_t data_zone_off, uint32_t current_data_zone, uint32_t zone_size, FILE *fs, uint32_t data_zone_num_off)
{
  uint32_t data_pos =data_zone_off + (
}

void getDblIndrZone(uint8_t *data, uint32_t current_zone, uint32_t *data_zone_off)
{

}
*/
