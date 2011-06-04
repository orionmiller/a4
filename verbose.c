#include "verbose.h"

void printSuperBlock(s_block *S_block)
{
   printf("Superblock Contents:\n");
   printf("Stored Fields:\n");
   printf("  ninodes%12u\n",S_block->ninodes);
   printf("  i_blocks%11u\n", S_block->imap_blocks);
   printf("  z_blocks%11u\n",S_block->zmap_blocks );
   printf("  firstdata%10u\n",S_block->firstdatazone);
   printf("  log_zone_size%6u ", S_block->log_zone_size);
   printf("(zone size: %u)\n",S_block->block_size<<S_block->log_zone_size);
   printf("  max_file%11u\n", S_block->max_size);
   printf("  magic%14x\n", S_block->magic);
   printf("  zones%14u\n", S_block->zones);
   printf("  blocksize%10u\n",S_block->block_size);
   printf("  subversion%9u\n\n",S_block->disk_version);
}

void verbose(s_block *S_block, inode *Inode)
{
  printSuperBlock(S_block);
  printInode(Inode);
}

void printInode(inode *Inode)
{
   uint8_t perms[PERM_SIZE+1];
   perms[PERM_SIZE]='\0'; /*null terminate the permissions string*/
   getPerm(Inode->mode, perms);
   printf("File inode:\n");
   printf("  uint16_t mode  %x (%s)\n",Inode->mode,perms);
   printf("  uint16_t links %u\n", Inode->nlinks);
   printf("  uint16_t uid %u\n", Inode->uid);
   printf("  uint16_t gid %u\n", Inode->gid);
   printf("  uint32_t size %u\n", Inode->size);
   printf("  uint32_t atime %u --- %s", Inode->atime, ctime((time_t *)&Inode->atime));
   printf("  uint32_t mtime %u --- %s", Inode->mtime, ctime((time_t *)&Inode->mtime));
   printf("  uint32_t ctime %u --- %s\n", Inode->ctime, ctime((time_t *)&Inode->ctime));

   printf("  Direct zones:\n");
   printf("              zone[0]   = %u\n", Inode->zone[0]);
   printf("              zone[1]   = %u\n", Inode->zone[1]);
   printf("              zone[2]   = %u\n", Inode->zone[2]);
   printf("              zone[3]   = %u\n", Inode->zone[3]);
   printf("              zone[4]   = %u\n", Inode->zone[4]);
   printf("              zone[5]   = %u\n", Inode->zone[5]);
   printf("              zone[6]   = %u\n", Inode->zone[6]);
   printf("  uint32_t    indirect    %u\n", Inode->ind_zone);
   printf("  uint32_t    double      %u\n", Inode->dbl_ind_zone);
}


void getPerm (uint16_t mode, uint8_t *perms)
{
   if(S_ISDIR(mode))
   {
      perms[0]='d';
   }
   else
   {
      perms[0]='-';
   }
   if((mode & S_IRUSR)==S_IRUSR)
   {
      perms[1]='r';
   }
   else
   {
      perms[1]='-';
   }
   if((mode & S_IWUSR)==S_IWUSR)
   {
      perms[2]='w';
   }
   else
   {
      perms[2]='-';
   }
   if((mode & S_IXUSR)==S_IXUSR)
   {
      perms[3]='x';
   }
   else
   {
      perms[3]='-';
   }
   if((mode & S_IRGRP)==S_IRGRP)
   {
      perms[4]='r';
   }
   else
   {
      perms[4]='-';

   }
   if((mode & S_IWGRP)==S_IWGRP)
   {
      perms[5]='w';
   }
   else
   {
      perms[5]='-';
   }
   if((mode & S_IXGRP)==S_IXGRP)
   {
      perms[6]='x';
   }
   else
   {
      perms[6]='-';
   }
   if((mode & S_IROTH)==S_IROTH)
   {
      perms[7]='r';
   }
   else
   {
      perms[7]='-';
   }
   if((mode & S_IWOTH)==S_IWOTH)
   {
      perms[8]='w';
   }
   else
   {
      perms[8]='-';
   }
   if((mode & S_IXOTH)==S_IXOTH)
   {
      perms[9]='x';
   }
   else
   {
      perms[9]='-';
   }
}
