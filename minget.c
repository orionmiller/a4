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
  int16_t part_num;
  int16_t subpart_num;
  uint8_t verbose;
  char *imagefile;
  char *path;
}options;


options * handleOptions(int argc, char *argv[]);

void printFile(FILE *output, uint8_t *data, uint32_t data_size)
{
 uint32_t pos;

 for (pos = 0; pos < data_size; pos++)
   {
     fprintf(output, "%c", data[pos]);
   }
 fflush(output);
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
  uint8_t *data = NULL;
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

  /*inode-pos = (2+imap_block+zmap_block)*blocksize;*/
  /*zone size = blocksize << log_zone_size*/
  /*data-pos = firstdatazone * zonesize*/

  if (Opt->part_num == -1 && Opt->subpart_num == -1) /*no partition or subpartition*/
    {
      zone_size = S_block->block_size << S_block->log_zone_size;
      inode_off = (2 + S_block->imap_blocks + S_block->zmap_blocks) * S_block->block_size;
      part_start = 0;
      output = stdout;
    }
  else if (0) /*partition but no subpartition*/
    {
      /*if valid partition num
       *  get partition table 
       *    check super block
       *    find start of part (part_off in relation to beginning of file)
       *    check super block
       *    find zone size
       *    find inode-offset
       */
    }
  else if (0) /* partition and subpartition */
    {
      /*if valid partition number & subpartition num
       *   get partition table
       *    find start of part (part_off in relation to beginning of file)
       *       check super block
       *       get sub partion table
       *          check super block
       *          find start of part (part_off in relation to beginning of file)
       *          find zone size
       *          find inode-offset
       */
    }
  else
    {
      printf("Bad Stuff\n"); /*change*/
    }
  path =tokstr(Opt->path, "/");
  if ((Inode = getFile(fs, path, inode_off, part_start, zone_size))==NULL)
    {
      fprintf(stderr, "File Doesn't Exist\n");
      exit(EXIT_FAILURE);
    }

  getData(fs, data, Inode, part_start, zone_size);
  printFile(output, data, Inode->size);  

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
	  break;

	case 's':
	  non_opt_args -= 2;
	  if (atoi(argv[optind]) < 0 || atoi(argv[optind]) > 3)
	    return NULL;
	  Opt->subpart_num = atoi(argv[optind]);
	  break;

	default:
	  return NULL;
	  break;
	}
    }


  if (Opt->subpart_num != -1 && Opt->part_num == -1)
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


  /*options check
  if (Opt->verbose)
    {
      printf("verbosity is on\n");
    }
  if (Opt->part_off != -1)
    {
      printf("partition offset: %d\n", Opt->part_off);
    }
  if (Opt->subpart_off != -1)
    {
      printf("sub partition offset: %d\n", Opt->subpart_off);
    }
  */


  /*

  if((Block = getSuperBlock(fs))==NULL)
    {
      fprintf(stderr, "Super Block Error\n");
      fclose(fs);
      exit(EXIT_FAILURE);
    }
  else
    {
      printf("Valid Super Block\n");
    }

  if((Block = getPartTable(fs, 0))==NULL)
    {
      fprintf(stderr, "Partition Table Error\n");
      fclose(fs);
      exit(EXIT_FAILURE);
    }
  else
    {
      printf("Is a partition!\n");
      printf("Parition Type: 0x%x\n", Block->part.type);

    }
  */

  /*
  if (Opt->part_num > -1 && Opt->part_num < 4)
    {
      P_table = getPartTable(fs, 0, Opt->part_num);
      if (!S_block)
	{
	  fprintf(stderr, "Partition Table Error\n");
	  exit(EXIT_FAILURE);
	}
      printf("Partition Size: %u\n", P_table->size);
    }
  */
