/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#include "fslib.h"

#include "partition.h"

p_table* getPartTable(FILE *fs, uint32_t offset, uint8_t p_table_num)
{
  p_table *P_table;
  uint8_t ptable_sig_1 = 0;
  uint8_t ptable_sig_2 = 0;
  uint32_t ptable_pos;

 /*partition table position*/
  ptable_pos = offset + PART_TABLE_BASE + (p_table_num*P_TABLE_SIZE);

  FATALCALL((P_table=(p_table *)malloc(P_TABLE_SIZE))==NULL,"partition: malloc");

  /*Check Partition Signature 1 - Magic Num Below*/
  FATALCALL(fseek(fs, offset + PART_SIG_1_OFF , SEEK_SET)==-1,"fseek");
  FATALCALL(fread((uint8_t *)&ptable_sig_1, 1, BYTE, fs) < BYTE,"fread");


  /*Check Partition Signature 2 - Magic Num Below*/
  FATALCALL(fseek(fs, offset + PART_SIG_2_OFF , SEEK_SET)==-1,"fseek");
  FATALCALL(fread((uint8_t *)&ptable_sig_2, 1, BYTE, fs) < BYTE, "fread")

  if (ptable_sig_1 != PART_SIG_1 || ptable_sig_2 != PART_SIG_2)
    {
      fprintf(stderr, "Incorrect Partition Signature(s)");
      fprintf(stderr, "Signature at 510 is (0x%X)\n", ptable_sig_1);
      fprintf(stderr, "Signature at 511 is (0x%X)\n", ptable_sig_2);
      return NULL;
    }

  /*Read in Partition Table*/
  FATALCALL(fseek(fs, ptable_pos, SEEK_SET)==-1,"fseek");
  FATALCALL(fread((p_table *)P_table, 1, P_TABLE_SIZE, fs) < P_TABLE_SIZE, "fread");
  

  /*Check Partition Table*/
  if (P_table->type != MINIX_PART)
    {
      fprintf(stderr, "Not Minix Part - Type: (0x%X)\n", P_table->type);
      return NULL;
    }

  return P_table;
}

s_block * getSuperBlock(FILE *fs, uint32_t offset)
{
  s_block *S_block;

  FATALCALL((S_block=(s_block *)malloc(S_BLOCK_SIZE))==NULL,"malloc");

  /*Check Super Block - Magic Num Below*/
  FATALCALL(fseek(fs, offset + S_BLOCK_BASE, SEEK_SET)==-1,"fseek");
  FATALCALL(fread((s_block *)S_block, 1, S_BLOCK_SIZE, fs) < S_BLOCK_SIZE, "fread");

  if(S_block->magic != S_BLOCK_MAGIC_LITTLE)
    {
      fprintf(stderr, "Super Block - Bad Magic Number");
      fprintf(stderr, "(0x%.2X)\n",S_block->magic);
      return NULL;
    }

  return S_block;
}

inode * getFile(FILE *fs, char ** path, uint32_t inode_off, uint32_t part_off, uint32_t zone_size)
{
  uint32_t inode_num = ROOT_INODE_NUM;

  uint8_t *dir_data;
  inode *Inode;
  uint32_t i;
  uint32_t dir_num_entries;
  FATALCALL((Inode=malloc(INODE_SIZE))==NULL,"malloc");

  getInode(Inode, ROOT_INODE_NUM, inode_off, fs);

  if(path[0] == NULL)/*magic number*/
    return Inode;

  for(i=0; path[i] != NULL; i++)
    {
      FATALCALL((dir_data=malloc(Inode->size))==NULL,"malloc");
      dir_data = getData(fs, Inode, part_off, zone_size);
      dir_num_entries = (Inode->size/DIR_ENTRY_SIZE);
      inode_num = existsInPath(dir_data, dir_num_entries, path[i]); 
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
  FATALCALL((fread((inode *)Inode, 1, INODE_SIZE, fs))!=INODE_SIZE,"fread");/*magic num*/
}


uint32_t existsInPath(uint8_t * dir_data, uint32_t dir_size, char *filename)
{
  uint8_t dir_entry_name[FILENAME_SIZE];
  uint32_t inode_num;

  uint8_t *dir_entry_pos = dir_data;

  while((dir_entry_pos - dir_data) < (dir_size*DIR_ENTRY_SIZE))
    {
      memcpy(dir_entry_name,dir_entry_pos+INODE_NUM_SIZE,FILENAME_SIZE);
      if(strncmp((char *)dir_entry_name,filename,FILENAME_SIZE)==0)
	{
	  memcpy(&inode_num, dir_entry_pos, INODE_NUM_SIZE);
	  return inode_num;
	}
      dir_entry_pos += DIR_ENTRY_SIZE;
    }
  return 0;
}

uint8_t * getData(FILE *fs, inode *Inode, uint32_t part_off, uint32_t zone_size)
{
  uint32_t filesize = Inode->size;
  uint32_t num_zones = filesize / zone_size;
  uint32_t current_zone = 0;
  uint32_t read;
  uint8_t *data;
  if (filesize%zone_size > 0)
    {
      num_zones++;
    }
  FATALCALL((data=malloc(num_zones*zone_size))==NULL, "malloc");

  for(read=0; current_zone < num_zones && read<9; read++)
    {
      if(read<7)
	{
	  getDirectZone(data, Inode->zone[read], part_off, &current_zone, zone_size, fs);
	}
      else if (read == 7)
	{
	  getIndrZone(data, Inode->ind_zone, part_off, &current_zone, zone_size, fs, num_zones);
	}
      else
	{
	  getDblZone(data, Inode->dbl_ind_zone, part_off, &current_zone, zone_size, fs, num_zones);
	}
    }
  return data;
} 


void getDirectZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs)
{
  uint32_t file_pos = part_off + (zone_ptr * zone_size);
  uint8_t * data_pos = data + (*current_zone * zone_size); /*make sure correct add*/

  FATALCALL(fseek(fs, file_pos, SEEK_SET)==-1,"fseek");
  FATALCALL((fread((uint8_t *)data_pos, 1, zone_size, fs))!=zone_size,"fread");/*magic num*/
  (*current_zone) += 1;
}

void getIndrZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs, uint32_t num_zones)
{
  uint32_t read;
  uint8_t *zone_ptr_block;
  uint32_t dummy_curr_zone = 0;
  FATALCALL((zone_ptr_block=(uint8_t*)malloc(zone_size))==NULL, "malloc");
  /*may cause problems double check*/
  getDirectZone(zone_ptr_block, zone_ptr, part_off, &dummy_curr_zone, zone_size, fs);
  
  for(read=0; *current_zone < num_zones && read < zone_size/INODE_SIZE; read++)
    {
      getDirectZone(data, zone_ptr_block[read], part_off, current_zone, zone_size, fs);
    }
  free(zone_ptr_block);
}

void getDblZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs, uint32_t num_zones)
{
  uint32_t read;
  uint8_t *indr_zone_ptr_block;
  uint32_t dummy_curr_zone = 0;
  FATALCALL((indr_zone_ptr_block=(uint8_t*)malloc(zone_size))==NULL, "malloc");
  /*may cause problems double check*/
  getDirectZone(indr_zone_ptr_block, zone_ptr, part_off, &dummy_curr_zone, zone_size, fs);
  
  /*may be truncating division error*/
  for(read=0; *current_zone < num_zones && read < zone_size/INODE_SIZE; read++)
    {
      getIndrZone(data, indr_zone_ptr_block[read], part_off, current_zone, zone_size, fs, num_zones);
    }
  free(indr_zone_ptr_block);
}
