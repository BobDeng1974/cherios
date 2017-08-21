/*-
 * Copyright (c) 2016 Hadrien Barral
 * Copyright (c) 2016 Lawrence Esswood
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include "cdefs.h"
#include "errno.h"
#include "types.h"
#include "nano/nanokernel.h"

#define NULL_PAIR (cap_pair){.code = NULL, .data = NULL}

typedef capability mop_t;

#define MOP_REQUIRED_SPACE (2 * RES_META_SIZE)

extern mop_t own_mop;

#define MEM_OK      (0)
#define MEM_BAD_MOP (-1)
#define MEM_BAD_BASE (-2)

#define MEM_REQUEST_NONE_FOUND  (-3)
#define MEM_REQUEST_UNAVAILABLE (-7)

#define MEM_CLAIM_NOT_IN_USE    (-4)
#define MEM_CLAIM_FREED         (-5)
#define MEM_CLAIM_CLAIM_LIMIT   (-6)

typedef enum mem_request_flags {
    NONE = 0,
    ALIGN_TOP = 1
} mem_request_flags;

/* 'You' is defined by a system given token "memory ownership principle" (mop). You must quote this to the system.
 * Each process should be given a mop.
 * Handing your mop to another principle is ok. */

/* Memory Request. Adds to your resource limit and claims the pages. Returns a reservation for those pages.
 * If align_top is set the range with have all its high bits the same.
 * base must be aligned to RES_META_SIZE. */
res_t       mem_request(size_t base, size_t length, mem_request_flags flags, mop_t mop);

/* Claiming adds to your resource limit - but guarantees the page(es) claimed will not be unmapped until you call release.
 * We must add to your resource limit straight away, otherwise we allow an attack where you think you are well below your
 * limit but then have lots of memory dumped on you. Note this does not award any capabilities. You can ask for any
 * page to stay mapped, not access any page. You must seek the capability elsewhere. */
int         mem_claim(size_t base, size_t length, mop_t mop);

/* Will give back your resource allowance, but will not guarantee unmapping. However, If you somehow still manage to
 * access the page it will still refer to the same physical page as before. */
int         mem_release(size_t base, size_t length, mop_t mop);

/* Makes a new mop, places it in space provided by a reservation, and returns a handle. */
mop_t       mem_makemop(res_t space, mop_t auth_mop);

/* Releases all resources attributable to mop and makes it invalid. Will also reclaim all mops derived from it */
int         mem_reclaim_mop(mop_t mop_sealed);

/* Creates the first mop, and also provides a sealing cap to the memory system. Can only be called once */
mop_t       init_mop(capability mop_sealing_cap);

/* Gets a physical capability. Can never be returned */
void        get_physical_capability(size_t base, size_t length, int IO, int cached, mop_t mop, cap_pair* result);

void commit_vmem(act_kt activation, size_t addr);

void	mmap_set_act(act_kt ref);
void    mmap_set_mop(mop_t mop);

void mdump(void);

size_t mvirtual_to_physical(size_t vaddr);

enum mmap_prot
{
  PROT_READ		= 1 << 0,
  PROT_WRITE		= 1 << 1,
  PROT_NO_READ_CAP	= 1 << 2,
  PROT_NO_WRITE_CAP	= 1 << 3,
  PROT_EXECUTE = 1 << 4
};
#define PROT_RW (PROT_READ | PROT_WRITE)

enum mmap_flags
{
  map_private	= 1 << 0,
  map_shared	= 1 << 1,
  map_anonymous	= 1 << 2,
};
#define MAP_PRIVATE map_private
#define MAP_ANONYMOUS map_anonymous
#define MAP_SHARED map_shared

enum mmap_return
{
    ENOMEM = 1
};

static cap_pair mmap_based_alloc(size_t s) {
    cap_pair p;
    res_t res = mem_request(0, s, NONE, own_mop);
    if(res == NULL) return NULL_PAIR;
    rescap_take(res, &p);
    return p;
}

static void mmap_based_free(capability c) {
    return mem_release(cheri_getbase(c), cheri_getlen(c), own_mop);
}

/* Old mmap for anything that needs it */
void *mmap(void *addr, size_t length, int prot, int flags, __unused int fd, __unused off_t offset);

int munmap(void *addr, size_t length);
#endif // SYS_MMAN_H
