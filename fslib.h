/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#ifndef FS_H
#define FS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "partition.h"
#include "super.h"
#include "inode.h"

#include "debug.h"
#include <string.h>
#define BASICBLOCK 1024
#define P_TABLE_SIZE 136
#define 
#define S_BLOCK_MAGIC 0x4D5A



/* Checks if there is a valid super block starting at byte 1024.
 * 
 * PARAMATER:
 *   fs - takes a file pointer to check the super block of.
 *
 * RETURN: On success it returns a pointer to the super block on failure
 *   returns NULL.
 * 
 * POSTCONDITION:
 *   The file offset will be different after use of the function.
 *
 * NOTES:
 *   The pointer to the table should be freed when done.
 */
super_block* checkSuperBlock(FILE *fs);


/* Takes a file pointer and checks if there is a valid partition table
 *   at 0x1BE.
 * 
 * PARAMATERS:
 *   fs - file pointer
 *
 * RETURN VALUE:
 *   SUCCESS - a pointer to the table is returned,
 *   FAILURE - returns NULL
 *
 * POSTCONDITION:
 *   The file offset will be different after use of the function.
 *
 * NOTES:
 *   The pointer to the table should be freed when done.
 */
part_entry* checkPartTable(FILE *fs);

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
 */
struct inode* fileChecker (FILE *fs, char* path, int inodeMap,int toInodes);


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
 */
unsigned char* getiData (struct inode *cNode, FILE *fs, int zonesize, 
			 int toDZones);



#endif