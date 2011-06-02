/*Structure Given from Assignment 4 Sec*/


#ifndef _PARTITION_H
#define _PARTITION_H

#include <stdint.h>

struct partition {
  uint8_t bootind;	/* boot indicator 0/ACTIVE_FLAG	 */
  uint8_t start_head;	/* head value for first sector	 */
  uint8_t start_sec;	/* sector value + cyl bits for first sector */
  uint8_t start_cyl;	/* track value for first sector	 */
  uint8_t type;
  uint8_t end_head;	/* head value for last sector	 */
  uint8_t end_sec;	/* sector value + cyl bits for last sector */
  uint8_t end_cyl;	/* track value for last sector	 */
  uint32_t lFirst;		/* logical first sector		 */
  uint32_t size;		/* size of partition in sectors	 */
};

#define P_TABLE_SIZE sizeof(struct partition)

#define ACTIVE_FLAG	0x80	/* value for active in bootind field (hd0) */
#define NR_PARTITIONS	4	/* number of entries in partition table */
#define	PART_TABLE_BASE	0x1BE	/* offset of partition table in boot sector */

/* Partition types. */
#define NO_PART		0x00	/* unused entry */
#define MINIX_PART	0x81	/* Minix partition type */

/*Partition Signatures */
#define PART_SIG_1 0x55
#define PART_SIG_1_OFF 0x1FE /*dec 510*/
#define PART_SIG_2 0xAA
#define PART_SIG_2_OFF 0x1FF /*dec 511*/

#endif /* _PARTITION_H */
