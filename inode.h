#ifndef INODE_H
#define INODE_H


#define NUM_ZONES 7
#define INODE_SIZE sizeof(struct inode_)
#define ZONE_PTR_SIZE sizeof(uint32_t)
#define ROOT_INODE_NUM

typedef struct inode_ {
  uint16_t mode;		/* mode_t - file type, protection, etc. */
  uint16_t nlinks;		/* nlink_t - how many links to this file */
  uint16_t uid;			/* uid_t - user id of the file's owner */
  uint16_t gid;			/* gid_t - group number */
  uint32_t size;			/* off_t - current file size in bytes */
  uint32_t atime;		/* time of last access (V2 only) */
  uint32_t mtime;		/* when was file data last changed */
  uint32_t ctime;		/* when was inode itself changed (V2 only)*/
  uint32_t zone[NUM_ZONES]; /* zone numbers for direct, ind, and dbl ind */
  uint32_t ind_zone;
  uint32_t dbl_ind_zone;
  uint32_t unused;
}inode;

#endif

/*OLD
struct inode {
  uint16_t mode;
  int16_t nlinks;
  int16_t uid;	
  uint16_t gid;	
  uint32_t size;
  time_t atime;
  time_t mtime;
  time_t ctime;
  uint32_t zone[NUM_ZONES]; 
  uint32_t ind_zone;
  uint32_t dbl_ind_zone;
  uint32_t unused;
};
*/
