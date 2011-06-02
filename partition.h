/*Structure Given from Assignment 4 Sec*/


#ifndef _PARTITION_H
#define _PARTITION_H

struct partition {
  unsigned char bootind;	/* boot indicator 0/ACTIVE_FLAG	 */
  unsigned char start_head;	/* head value for first sector	 */
  unsigned char start_sec;	/* sector value + cyl bits for first sector */
  unsigned char start_cyl;	/* track value for first sector	 */
  unsigned char type;
  unsigned char end_head;	/* head value for last sector	 */
  unsigned char end_sec;	/* sector value + cyl bits for last sector */
  unsigned char end_cyl;	/* track value for last sector	 */
  unsigned long lFirst;		/* logical first sector		 */
  unsigned long size;		/* size of partition in sectors	 */
};

#define ACTIVE_FLAG	0x80	/* value for active in bootind field (hd0) */
#define NR_PARTITIONS	4	/* number of entries in partition table */
#define	PART_TABLE_OFF	0x1BE	/* offset of partition table in boot sector */

/* Partition types. */
#define NO_PART		0x00	/* unused entry */
#define MINIX_PART	0x81	/* Minix partition type */

/*Partition Signatures */
#define PART_SIG_1 0x55
#define PART_SIG_1_OFF 510
#define PART_SIG_2 0xAA
#define PART_SIG_2_OFF 511

#endif /* _PARTITION_H */
