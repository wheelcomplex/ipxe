#ifndef VIRTADDR_H
#define VIRTADDR_H

/* Segment selectors as used in our protected-mode GDTs.
 *
 * Don't change these unless you really know what you're doing.
 */

#define VIRTUAL_CS 0x08
#define VIRTUAL_DS 0x10
#define PHYSICAL_CS 0x18
#define PHYSICAL_DS 0x20
#define REAL_CS 0x28
#define REAL_DS 0x30
#if 0
#define LONG_CS 0x38
#define LONG_DS 0x40
#endif

#ifndef ASSEMBLY

#ifndef KEEP_IT_REAL

/*
 * Without -DKEEP_IT_REAL, we are in 32-bit protected mode with a
 * fixed link address but an unknown physical start address.  Our GDT
 * sets up code and data segments with an offset of virt_offset, so
 * that link-time addresses can still work.
 *
 */

/* Variables in virtaddr.S */
extern unsigned long virt_offset;

#else /* KEEP_IT_REAL */

#include <stdint.h>

/*
 * With -DKEEP_IT_REAL, we are in 16-bit real mode with fixed link
 * addresses and a segmented memory model.  We have separate code and
 * data segments.
 *
 * Because we may be called in 16-bit protected mode (damn PXE spec),
 * we cannot simply assume that physical = segment * 16 + offset.
 * Instead, we have to look up the physical start address of the
 * segment in the !PXE structure.  We have to assume that
 * virt_to_phys() is called only on pointers within the data segment,
 * because nothing passes segment information to us.
 *
 * We don't implement phys_to_virt at all, because there will be many
 * addresses that simply cannot be reached via a virtual address when
 * the virtual address space is limited to 64kB!
 */

static inline unsigned long virt_to_phys ( volatile const void *virt_addr ) {
	/* Cheat: just for now, do the segment*16+offset calculation */
	uint16_t ds;

	__asm__ ( "movw %%ds, %%ax" : "=a" ( ds ) : );
	return ( 16 * ds + ( ( unsigned long ) virt_addr ) );
}

/* Define it as a deprecated function so that we get compile-time
 * warnings, rather than just the link-time errors.
 */
extern void * phys_to_virt ( unsigned long phys_addr )
     __attribute__ ((deprecated));

#endif /* KEEP_IT_REAL */

#endif /* ASSEMBLY */

#endif /* VIRTADDR_H */
