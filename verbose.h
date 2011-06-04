#ifndef _VERBOSE_H
#define _VERBOSE_H

#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "super.h"
#include "inode.h"

#define PERM_SIZE 10

void getPerm(uint16_t mode, uint8_t *perms);
void printSuperBlock(s_block *S_block);
void printInode(inode *Inode);
void verbose(s_block *S_block, inode *Inode);

#endif
