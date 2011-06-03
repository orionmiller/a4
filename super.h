#ifndef _SUPER_H
#define _SUPER_H

typedef struct super_block {
  uint32_t ninodes;		/* # usable inodes on the minor device */
  uint16_t nzones;		/* total device size, including bit maps etc */
  int16_t imap_blocks;		/* # of blocks used by inode bit map */
  int16_t zmap_blocks;		/* # of blocks used by zone bit map */
  uint16_t firstdatazone;	/* number of first data zone */
  int16_t log_zone_size;	/* log2 of blocks/zone */
  int16_t pad;			/* try to avoid compiler-dependent padding */
  uint32_t max_size;		/* maximum file size on this device */
  uint32_t zones;		/* number of zones (replaces nzones in V2) */
  int16_t magic;		/* magic number to recognize super-blocks */

  /* The following items are valid on disk only for V3 and above */

  /* The block size in bytes. Minimum MIN_BLOCK SIZE. SECTOR_SIZE
   * multiple. If V1 or V2 filesystem, this should be
   * initialised to STATIC_BLOCK_SIZE. Maximum MAX_BLOCK_SIZE.
   */
  int16_t pad2;			/* try to avoid compiler-dependent padding */
  uint16_t block_size;	/* block size in bytes. */
  char disk_version;		/* filesystem format sub-version */

}s_block;

/*#define NIL_SUPER (struct super_block *) 0*/
#define S_BLOCK_BASE     1024                         /*Super Block File Offset*/
#define S_BLOCK_SIZE    (sizeof(struct super_block))
#define S_BLOCK_MAGIC_LITTLE   0x4D5A /*magic num on little endian machine*/
#define S_BLOCK_MAGIC_BIG 0x5A4D /*magic num on big endian machine*/
#define IMAP		0	/* operating on the inode bit map */
#define ZMAP		1	/* operating on the zone bit map */
#endif
