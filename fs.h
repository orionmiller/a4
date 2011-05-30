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
 * PARAM: Takes a file pointer.
 * RETURN: On success it returns a pointer to the super block on failure
 *   returns NULL.
 * 
 * pointer should be freed when done
 * position of fs will be different after use
 */
super_block* checkSuperBlock(FILE *fs);


/* Takes a file pointer and checks if there is a valid partition table
 *   at 0x1BE.
 * On success a pointer to the table is returned, on failure returns NULL
 * pointer to table should be freed when done
 * position of fs will be different after use
 */
part_entry* checkPartTable(FILE *fs);

/* may change to return a regular or directory file*/
/* Checks that the path specified is valid and that the last item is
 *   a regular file. 
 *
 * PARAM:
 *   fs - the filestream that contains the filesystem
 *   inodeMap - the offset from the begining of the fs that the inodemap 
 *     starts on. 
 *   toInodes - the number of bytes to get from the begining
 *     of the inode map to the first inode.
 *
 * RETURN:
 *   not valid - returns a -1
 *   valid - returns the offset to the listed file's inode from the begining
 *      of the Inodes(Inode# * 64).
 */
struct inode* fileChecker (FILE *fs, char* path, int inodeMap,int toInodes);


/* Gets all of the data pointed to in the inode.
 * RETURN: 
 *   valid - a pointer to a buffer with all of it.
 *   invalid - NULL
 */
unsigned char* getiData (struct inode* cNode,FILE *fs, int zonesize, 
			 int toDZones);



#endif
