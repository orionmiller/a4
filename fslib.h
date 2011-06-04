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
#include "toklib.h"
/*
#include "super.h"
#include "inode.h"
*/
#include "debug.h"

#define BASICBLOCK 1024
#define SECTOR_SIZE 512
#define S_BLOCK_MAGIC 0x4D5A
#define FILENAME_SIZE 60
#define INODE_NUM_SIZE (sizeof(uint32_t))
#define DIR_ENTRY_SIZE (FILENAME_SIZE+INODE_NUM_SIZE)
#define BYTE (sizeof(uint8_t))

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
s_block* getSuperBlock(FILE *fs, uint32_t offset);

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
p_table* getPartTable(FILE *fs, uint32_t offset, uint8_t p_table_num);

inode * getFile(FILE *fs, char ** path, uint32_t inode_off, uint32_t part_off, uint32_t zone_size);

void getInode(inode * Inode, uint32_t inode_num, uint32_t inode_off, FILE *fs);


uint32_t existsInPath(uint8_t * dir_data, uint32_t dir_size, char *filename);


uint8_t * getData(FILE *fs, inode *Inode, uint32_t part_off, uint32_t zone_size);


void getDirectZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs);
void getIndrZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs, uint32_t num_zones);
void getDblZone(uint8_t *data, uint32_t zone_ptr, uint32_t part_off, uint32_t *current_zone, uint32_t zone_size, FILE *fs, uint32_t num_zones);

#endif
