/* Authors: Geoffrey Lawson, Orion Miller
 * Class: CPE 453
 * Assignment 4
 */

#include "fs.h"



super_block* checkSuperBlock(FILE *fs)
{
   super_block* sBlock;
   FATALCALL((sBlock=malloc(BASICBLOCK))==NULL,"malloc");
   FATALCALL(fseek(fs, BASICBLOCK, SEEK_SET)==-1,"fseek");
   FATALCALL(fread((void*)sBlock, sizeof(char),BASICBLOCK,fs)!=BASICBLOCK,
         "fread");
   if(sBlock->magic==S_BLOCK_MAGIC)
   {
      return sBlock;
   }
   return NULL;
}


part_entry* checkPartTable(FILE *fs)
{
  /* part_entry* pTable;*/
   char byte1, byte2;
   /*PTABLESIZE is a guess right now, the struct has 2 longs and 8 chars
    * so that seemed like 136 bytes, needs further research
    *
    *
    *
   FATALCALL((pTable=malloc(PTABLESIZE))==NULL,"malloc");
   FATALCALL(fseek(fs, 0x1BE, SEEK_SET)==-1,"fseek");
   FATALCALL(fread((void*)pTable, sizeof(char),PTABLESIZE,fs)!=PTABLESIZE,
         "fread");
*/
   /*get bytes 510 and 511, if they equal the magic numbers we have a 
    * valid partition table. Still need to figure out how to get the table
    * this can be postponed, this function only needs to work once we have the basic
    * nonpartitioned version working
    */
   FATALCALL(fseek(fs, 510, SEEK_SET)==-1,"fseek");
   FATALCALL(fread((void*)&byte1, sizeof(char),1,fs)!=1,
         "fread");
   if(byte1!=0x55)
   {
      return NULL;
   }
   FATALCALL(fread((void*)&byte2, sizeof(char),1,fs)!=1,
         "fread");
   if(byte2!=0xAA)
   {
      return NULL;
   }

}


struct inode* fileChecker (FILE *fs, char* path, int inodeMap,int toInodes)
{
   int iNum=1, numEnts;
   char iName[60]="root"; /*magic NUM*/
   char *nextName, *vicPath;
   struct inode* cNode;
   /* set up a copy of path to be mangled by strtok*/
   FATALCALL((vicPath=malloc(strlen(path)+1))==NULL,"malloc");
   FATALCALL((cNode=malloc(64))==NULL,"malloc"); /*magic NUM*/

   strcpy(vicPath,path);

   FATALCALL(fseek(fs, inodeMap, SEEK_SET)==-1,"fseek");
   /*get us to the right starting inode entry*/
   if(path[0]=='/')
   {
     /*get to the right directory if it's not root
      * should probably go to .. from root all starting directories
      * can be accessed from there
      */
   }
   else
   {
      /*set up the strtoking*/
      nextName=strtok(vicPath,"/\0");
   }
   /*while the current inode entry corresponds to a valid file*/
   while(iNum!=0)
   {
      /*seek to the right inode*/
      FATALCALL(fseek(fs,inodeMap+toInodes+(64*iNum),SEEK_SET)==-1,"fseek");
      FATALCALL(fread((void*)cNode,sizeof(unsigned char),64,fs)==-1,"fread");
      /*if cNode is for a normal file*/
      if(cNode->i_mode&0170000!=0040000)
      {
         /*if it's the last thing in the path, that's ok*/
         if(strtok(NULL,"/\0")==NULL)
         {
            return cNode;
         }
         else /*non directory files can't be in the middle of a path name*/
         {
            return NULL;
         }
      }
      numEnts=(cNode->i_size)/64;
      
      /* dear god where is the inode struct defined it would make this next step
       * easier. general idea is to memcpy the next 64 bytes into the inode 
       * struct find out if it's a a directory or not, confirm thats what we 
       * think it should be. next we go through the listing of files if it's
       * a directory and find the inode number matching nextName. get the next
       * next name with strtok, then loop. when we get our normal file, return 
       * it's offset. break at any problem.
       */
   }
   return(NULL);
}


unsigned char* getiData (struct inode* cNode,FILE *fs, int zonesize, 
      int toDZones)
{
   unsigned char* data
   unsigned int bytesLeft=cNode->i_size;
   int cZone=0;/*what number zone we are in on the inode, NOT IN THE DATA*/
   FATALCALL((data=malloc(cNode->i_size))==NULL,"malloc");
   while(bytesLeft>zonesize)
   {
      /*dealing with the direct blocks*/
      if(cZone<=7)
      {
         FATALCALL(fseek(fs,toDZones+(zonesize*cNode->i_zone[cZone]),
                  SEEK_SET)==-1,"fseek");
         FATALCALL(fread((void*)&data[cZone],sizeof(unsigned char),zonesize
                  ,fs)==-1,"fread");
         bytesLeft-=zonesize;
      }
      else
      {
         
      }
   }
}

