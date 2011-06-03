/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 *
 * Main Function for minget and arguments parsing.
 */

#include <stdio.h>
#include <stdlib.h>
#include "fslib.h"
#include "debug.h"
#include <getopt.h>
#include <unistd.h>


#define OPT_STR "psv"
#define OPTIONS_SIZE (sizeof(struct param_options))

typedef struct param_options {
  int16_t part_num;
  int16_t subpart_num;
  uint8_t verbose;
  uint8_t *imagefile;
  uint8_t *path;
}options;


options * handleOptions(int argc, char *argv[]);


int main(int argc, char *argv[])
{
  s_block *S_block;
  p_table *P_table;
  inode *Inode;
  options *Opt;
  FILE *fs;
  uint32_t offset; 

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
  /*needed data from super block
   * block_size
   * zone_size 
   */
  /*inode-pos = (2+imap_block+zmap_block)*blocksize;*/
  /*zone size = blocksize << log_zone_size*/
  /*data-pos = firstdatazone * zonesize*/


  /*if valid partition num
   *  get partition table 
   *    find start of part (part_off in relation to beginning of file)
   *    check super block
   *    find zone size
   *    find inode-offset
   */
  /*if valid partition number & subpartition num
   *   get partition table
   *    find start of part (part_off in relation to beginning of file)
   *       check super block
   *       get sub partion table
   *          find start of part (part_off in relation to beginning of file)
   *          check super block
   *          find zone size
   *          find inode-offset
   */

  if (Opt->part_num > -1 && Opt->part_num < 4)
    {
      S_block = getPartTable(fs, 0, Opt->part_num); /*MAGIC NUMBER*/
      if (!S_block)
	{
	  fprintf(stderr, "Partition Table Error\n");
	  exit(EXIT_FAILURE);
	}
      printf("Partition Size: %u\n", P_table->size);
    }

  /*acquire sub partition stuff*/

  

  fclose(fs); /*check for errors*/
  free(P_table);
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
  Opt->part_off = -1;
  Opt->subpart_off = -1;
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
	  Opt->part_off = atoi(argv[optind]);
	  break;

	case 's':
	  non_opt_args -= 2;
	  if (atoi(argv[optind]) < 0 || atoi(argv[optind]) > 3)
	    return NULL;
	  Opt->subpart_off = atoi(argv[optind]);
	  break;

	default:
	  return NULL;
	  break;
	}
    }


  if (Opt->subpart_off != -1 && Opt->part_off == -1)
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
