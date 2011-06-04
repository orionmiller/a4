/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 *
 * Main Function for minget and arguments parsing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include "fslib.h"
#include "debug.h"
#include "verbose.h"

#define OPT_STR "psv"
#define OPTIONS_SIZE (sizeof(struct param_options))

typedef struct param_options {
  uint8_t part;
  int16_t part_num;
  uint8_t subpart;
  int16_t subpart_num;
  uint8_t verbose;
  char *imagefile;
  char *path;
}options;

options * handleOptions(int argc, char *argv[]);

void printDir(FILE *fs, inode * dirInode, uint8_t *dir_data, uint32_t inode_off, char* path)
{
  uint8_t dir_entry_name[FILENAME_SIZE + 1];
  uint8_t *dir_entry_pos = dir_data;
  uint32_t inode_num;
  uint32_t dir_num_entries;
  inode * entry_Inode;
  uint8_t perms[PERM_SIZE+1];
  FATALCALL((entry_Inode=malloc(INODE_SIZE))==NULL,"malloc");
  perms[PERM_SIZE]='\0'; /*null terminate the permissions string*/
  dir_num_entries = (dirInode->size / DIR_ENTRY_SIZE);
  dir_entry_name[FILENAME_SIZE] = '\0';
  printf("%s:\n", path);
  while((dir_entry_pos - dir_data) < (dir_num_entries*DIR_ENTRY_SIZE))
    {
      memcpy(&inode_num, dir_entry_pos, INODE_NUM_SIZE);
      if (inode_num > 0)
	{
	  memcpy(dir_entry_name,dir_entry_pos+INODE_NUM_SIZE,FILENAME_SIZE);
	  getInode(entry_Inode, inode_num, inode_off, fs);
	  getPerm(entry_Inode->mode, perms);
	  printf("%s %9d %s\n", perms, entry_Inode->size, dir_entry_name);
	}
      dir_entry_pos += DIR_ENTRY_SIZE;
    }
}


int main(int argc, char *argv[])
{
  s_block *S_block;
  p_table *P_table=NULL;
  inode *Inode = NULL;
  options *Opt;
  FILE *fs;
  FILE *output; /*where all the data will be outputed*/
  uint32_t part_start; /*beginning file system*/
  uint32_t inode_off; /*beginning of inode zone*/
  uint32_t zone_size;
  uint8_t *data;
  char **path; /*tokenize path*/
 

  if ((Opt = handleOptions(argc, argv))==NULL)
    {
      printf("minls [-v] [-p part [-s subpart]] imagefile [path]\n");
      exit(EXIT_FAILURE);
    }

  FATALCALL((fs = fopen(Opt->imagefile, "rb"))==NULL,"fopen");

  part_start = 0;
  output = stdout;

  if (!(Opt->part) && !(Opt->subpart)) /*no partition or subpartition*/
    {
      if((S_block = getSuperBlock(fs, Opt->part_num)) == NULL) /*MAGIC NUMBER*/
	{
	  free(S_block);
	  exit(EXIT_FAILURE);
	}
      zone_size = S_block->block_size << S_block->log_zone_size;
      inode_off = part_start + (2 + S_block->imap_blocks + S_block->zmap_blocks)
	* S_block->block_size;

    }
  else if ((Opt->part) && !(Opt->subpart)) /*partition but no subpartition*/
    {
      P_table = getPartTable(fs, part_start, Opt->part_num);
      if (P_table == NULL)
	{
	  free(P_table);
	  exit(EXIT_FAILURE);
	}

      part_start = P_table->lFirst * SECTOR_SIZE;
      if((S_block = getSuperBlock(fs, part_start)) == NULL) /*MAGIC NUMBER*/
	{
	  free(S_block);
	  exit(EXIT_FAILURE);
	}
      zone_size = S_block->block_size << S_block->log_zone_size;
      inode_off = part_start + (2 + S_block->imap_blocks + S_block->zmap_blocks)
	* S_block->block_size;
    }
  else if (Opt->part && Opt->subpart) /* partition and subpartition */
    {
      P_table = getPartTable(fs, part_start, Opt->part_num); /*0 is start of image*/
      if (P_table == NULL)
	{
	  free(P_table);
	  exit(EXIT_FAILURE);
	}

      part_start = P_table->lFirst * SECTOR_SIZE;
      free(P_table);
      P_table = getPartTable(fs, part_start, Opt->subpart_num);
      if (P_table == NULL)
	{
	  free(P_table);
	  exit(EXIT_FAILURE);
	}
      part_start = P_table->lFirst * SECTOR_SIZE;
      if((S_block = getSuperBlock(fs, part_start)) == NULL) /*MAGIC NUMBER*/
	{
	  free(S_block);
	  exit(EXIT_FAILURE);
	}
      zone_size = S_block->block_size << S_block->log_zone_size;
      inode_off = part_start + (2 + S_block->imap_blocks + S_block->zmap_blocks) * S_block->block_size;
    }
  else
    {
      exit(EXIT_FAILURE);
    }

  path =tokstr(Opt->path, "/");
  if ((Inode = getFile(fs, path, inode_off, part_start, zone_size))==NULL)
    {
      fprintf(stderr, "File Doesn't Exist\n");
      exit(EXIT_FAILURE);
    }

  data = getData(fs, Inode, part_start, zone_size);
  if (Opt->verbose)
    verbose(S_block, Inode);
  printDir(fs, Inode, data, inode_off, Opt->path);

  fclose(fs); /*check for errors*/
  if(P_table!=NULL)
    {  
      free(P_table);
    }
  free(Opt);
  free(S_block);
  return EXIT_SUCCESS;
}


options * handleOptions(int argc, char *argv[])
{
  uint32_t opt;
  options *Opt;
  extern int optind;
  uint32_t non_opt_args = argc; /*number of non - option arguments*/

  FATALCALL((Opt=(options *)malloc(OPTIONS_SIZE))==NULL, "malloc");
  Opt->part_num = -1;
  Opt->subpart_num = -1;
  Opt->verbose = 0;
  Opt->imagefile = NULL;
  Opt->path = NULL;
  Opt->part = 0;
  Opt->subpart = 0;

  while ((opt = getopt(argc, argv, OPT_STR)) != -1)
    {
      switch (opt)
	{
	case 'v':
	  non_opt_args -= 1;
	  Opt->verbose = 1;
	  break;

	case 'p':
	  non_opt_args -= 2;
	  if (atoi(argv[optind]) < 0 || atoi(argv[optind]) > 3)
	    return NULL;
	  Opt->part_num = atoi(argv[optind]);
	  Opt->part = 1;
	  break;

	case 's':
	  non_opt_args -= 2;
	  if (atoi(argv[optind]) < 0 || atoi(argv[optind]) > 3)
	    return NULL;
	  Opt->subpart_num = atoi(argv[optind]);
	  Opt->subpart = 1;
	  break;

	default:
	  return NULL;
	  break;
	}
    }

  if (Opt->subpart == 1 && Opt->part == 0)
    {
      return NULL;
    }

  switch (non_opt_args)
    {
    case 2: /*imagefile is only provided by user*/
      Opt->imagefile = argv[argc-1];
      FATALCALL((Opt->path=(char *)calloc(2*sizeof(char), 
        2*sizeof(char)))==NULL, "calloc");
      Opt->path[0] = '/';
      break;

    case 3: /*imigfile and path is provided by user*/
      Opt->imagefile = argv[argc-2];
      Opt->path = argv[argc-1];
      break;

    default:
      return NULL;
      break;
    }

  return Opt;
}
