/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#ifndef FS_H
#define FS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "super.h"
#include "inode.h"
#include "partition.h"
/*
#include "super.h"
#include "inode.h"
*/
#include "debug.h"

#define BASICBLOCK 1024

#define S_BLOCK_MAGIC 0x4D5A
#define FILENAME_SIZE 60
#define INODE_NUM_SIZE sizeof(uint32_t)
#define FILE_ENTRY_SIZE (FILENAME_SIZE+INODE_NUM_SIZE)

typedef union generic_block{
  struct partition p_table;
  struct super_block s_block;
}block;

/* Takes a file pointer and checks if there is a valid super block starting 
 *   at byte 1024. If it is valid it returns the super block data.
 * 
 * PARAMATER:
 *   fs - takes a file pointer to check the super block of.
 *
 * RETURN:
 *   SUCCESS -  pointer to union super block
 *   FAILURE - returns NULL.
 * 
 * POSTCONDITION:
 *   The file offset will be different after use of the function.
 *
 * NOTES:
 *   The pointer to the table should be freed when done.
 */
block * getSuperBlock(FILE *fs, uint32_t offset);

/* Takes a file pointer and checks if there is a valid partition table
 *   at 0x1BE + offset and returns the partition table data.
 * 
 * PARAMATERS:
 *   fs - file pointer
 *   part_off - the partition table offset
 *
 * RETURN VALUE:
 *   SUCCESS - a pointer to the partition table data is returned
 *   FAILURE - returns NULL
 *
 * POSTCONDITION:
 *   The file offset will be different after use of the function.
 *
 * NOTES:
 *   The pointer to the table should be freed when done.
 */
block* getPartTable(FILE *fs, uint32_t offset, uint8_t p_table_num);


/* Checks to make sure fread() did not return an EOF or set ERRNO.
 *  
 * PARAMATERS:
 *   read_size = the read in size from fread().
 *   correct_size = the size fread() should have returned.
 *   fs - file pointer
 *
 * RETURN VALUE:
 *   SUCCESS - returns 1
 *   FAILURE - returns 0
 *
 * NOTES:
 *   The file should be closed when it is no longer needed.
 */
int  correctRead(size_t read_size, size_t correct_size, FILE *fs);


/* may change to return a regular or directory file*/
/* Checks that the path specified is valid and that the last item is
 *   a regular file. 
 *
 * PARAMATERS:
 *   fs - the filestream that contains the filesystem
 *   inodeMap - the offset from the begining of the fs that the inodemap 
 *     starts on. 
 *   toInodes - the number of bytes to get from the begining
 *     of the inode map to the first inode.
 *
 * RETURN VALUE:
 *   SUCCESS - returns a -1
 *   FAILURE - returns the offset to the listed file's inode from the begining
 *      of the Inodes(Inode# * 64).

struct inode* fileChecker (FILE *fs, char* path, int inodeMap,int toInodes);
 */

/* Gets all of the data pointed to in the inode.
 *
 * PARAMATERS:
 *   cNode - 
 *   fs - 
 *   zonesize -
 *   toDZones - 
 *
 * RETURN VALUE: 
 *   SUCCESS - pointer to a buffer all the data read in.
 *   FAILURE - returns NULL pointer

unsigned char* getiData (struct inode *cNode, FILE *fs, int zonesize, 
			 int toDZones);
 */


#endif
