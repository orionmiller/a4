#include <stdio.h>
#include <stdlib.h>
#include "fslib.h"
#include "debug.h"
#include <getopt.h>

#define OPT_STR "psv"
#define OPTIONS_SIZE (sizeof(struct param_options))

typedef struct param_options {
  uint8_t part_off;
  uint8_t subpart_off;
  uint8_t verbose;
  char * imagefile;
  char * path;
}options;


options * handleOptions(int argc, char *argv[]);


int main(int argc, char *argv[])
{
  block *Block;
  options *Opt;
  FILE *fs;
  
  if ((Opt = handleOptions(argc, argv))==NULL)
    {
      printf("minls [-v] [-p part [-s subpart]] imagefile [path]\n");
    }
  
  FATALCALL((fs = fopen(Opt->imagefile, "rb"))==NULL,"fopen");
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
      free(Block);
    }
  fclose(fs);
  return EXIT_SUCCESS;
}


options * handleOptions(int argc, char *argv[])
{
  uint32_t opt;
  options *Opt;

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
	  Opt->verbose = 1;
	  break;

	case 'p':
	  if (atoi(optarg) < 0 || atoi(optarg) > 3)
	    return NULL;
	  Opt->part_off = atoi(optarg);
	  break;

	case 's':
	  if (atoi(optarg) < 0 || atoi(optarg) > 3)
	    return NULL;
	  Opt->subpart_off = atoi(optarg);
	  break;

	default:
	  return NULL;
	}
    }

  if (Opt->subpart_off != -1 && Opt->part_off == -1)
    return NULL;

  switch (argc)
    {
    case 1:
      Opt->imagefile = argv[1];
      FATALCALL((Opt->path=(char *)calloc(2*sizeof(char), 
        2*sizeof(char)))==NULL, "calloc");
      Opt->path[0] = '/';
      break;

    case 2:
      Opt->imagefile = argv[1];
      Opt->path = argv[2];
      break;
     
    default:
      return NULL;
      break;
    }

  return Opt;
}
