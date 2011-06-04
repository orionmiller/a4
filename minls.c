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

void printDir(FILE *output, uint8_t *data, uint32_t data_size)
{
 uint32_t pos;
 uint32_t c_pos;

 for (pos = 0; pos < data_size; pos += DIR_ENTRY_SIZE)
   {
     for (c_pos = 0; data[c_pos+pos+INODE_NUM_SIZE] != '\0' && c_pos < FILENAME_SIZE; c_pos++)
       {
	 putchar(data[c_pos+pos+INODE_NUM_SIZE]);
       }
     printf("\n");
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
  char **path;
 

  if ((Opt = handleOptions(argc, argv))==NULL)
    {
      printf("minls [-v] [-p part [-s subpart]] imagefile [path]\n");
      exit(EXIT_FAILURE);
    }

  FATALCALL((fs = fopen(Opt->imagefile, "rb"))==NULL,"fopen");

  /*check super block*/
  if((S_block = getSuperBlock(fs, 0)) == NULL) /*MAGIC NUMBER*/
    {
      free(S_block);
      exit(EXIT_FAILURE);
    }

  part_start = 0;
  output = stdout;

  if (Opt->part_num == -1 && Opt->subpart_num == -1) /*no partition or subpartition*/
    {
      zone_size = S_block->block_size << S_block->log_zone_size;
      inode_off = part_start + (2 + S_block->imap_blocks + S_block->zmap_blocks) * S_block->block_size;

    }
  else if (Opt->part && !Opt->subpart) /*partition but no subpartition*/
    {
      P_table = getPartTable(fs, part_start, Opt->part_num);
      if (P_table == NULL)
	exit(EXIT_FAILURE);
      
      part_start = P_table->lFirst * SECTOR_SIZE;
      free(S_block);
      S_block = getSuperBlock(fs, part_start);
      
      if (S_block == NULL)
	exit(EXIT_FAILURE);
      
      zone_size = S_block->block_size << S_block->log_zone_size;
      inode_off = part_start + (2 + S_block->imap_blocks + S_block->zmap_blocks) * S_block->block_size;
    }
  else if (Opt->part && Opt->subpart) /* partition and subpartition */
    {
      P_table = getPartTable(fs, part_start, Opt->part_num); /*0 is start of image*/
      if (P_table == NULL)
	  exit(EXIT_FAILURE);

      part_start = P_table->lFirst * SECTOR_SIZE;

      free(S_block);
      S_block = getSuperBlock(fs, part_start);
      if (S_block == NULL)
	  exit(EXIT_FAILURE);

      free(P_table);
      P_table = getPartTable(fs, part_start, Opt->subpart_num);
      if (P_table == NULL)
	  exit(EXIT_FAILURE);

      part_start += P_table->lFirst * SECTOR_SIZE;
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
  printf("zone size: %u\n", zone_size);
  printf("inode size: %u\n", Inode->size);
  printf("zone 0 ptr: %u\n", Inode->zone[0]);
  data = getData(fs, Inode, part_start, zone_size);

  printDir(output, data, Inode->size);  

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
