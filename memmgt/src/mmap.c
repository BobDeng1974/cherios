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

#include "lib.h"
#include "sys/mman.h"
#include "types.h"

char * pool = NULL;
static size_t pages_nb = 0;

typedef enum e_page_status {
	page_unused,
	page_used,
	page_released,
	page_child
} e_page_status;

typedef struct {
	e_page_status	status;
	act_kt 	owner; /* activation owner */
	size_t	len; /* number of pages in this chunk */
	size_t	prev; /* start of previous chunk */
	size_t 	next;
} page_t;

static page_t * book = NULL;

/* fd and offset are currently unused and discarded in userspace */
void *__mmap(void *addr, size_t length, int prot, int flags) {
	int perms = CHERI_PERM_SOFT_1; /* can-free perm */
	if(addr != NULL)
		panic("mmap: addr must be NULL");

	if(!(flags & MAP_ANONYMOUS)) {
		errno = EINVAL;
		goto fail;
	}
	if((flags & MAP_PRIVATE) && (flags & MAP_SHARED)) {
		errno = EINVAL;
		goto fail;
	}

	if(flags & MAP_PRIVATE) {
		perms |= CHERI_PERM_STORE_LOCAL_CAP;
	} else if(flags & MAP_SHARED) {
		perms |= CHERI_PERM_GLOBAL;
	} else {
		errno = EINVAL;
		goto fail;
	}

	if(prot & PROT_READ) {
		perms |= CHERI_PERM_LOAD;
		if(!(prot & PROT_NO_READ_CAP))
			perms |= CHERI_PERM_LOAD_CAP;
	}
	if(prot & PROT_WRITE) {
		perms |= CHERI_PERM_STORE;
		if(!(prot & PROT_NO_WRITE_CAP))
			perms |= CHERI_PERM_STORE_CAP;
	}

	if(prot & PROT_EXECUTE) {
		perms |= CHERI_PERM_EXECUTE;
	}

	void * p = NULL;

	size_t pages_wanted = length/pagesz;
	if(pages_wanted*pagesz < length)
		pages_wanted++;

	assert(pages_wanted*pagesz >= length);

	/* fixme: fix for dlmalloc so it cannot try to merge chunks of memory */
	pages_wanted++;

	/* find some available space */
	size_t page = 0;
	while(page < pages_nb) {
		if(book[page].status != page_unused)
			page += book[page].len;
		else if(book[page].len < pages_wanted)
			page += book[page].len;
		else
			goto found;
	}
	goto fail;

 found:
	/* update mapping */
	book[page].status = page_used;
	size_t curr_len = book[page].len;
	book[page].len = pages_wanted;
	if(pages_wanted < curr_len) {
		book[page+pages_wanted].status = page_unused;
		book[page+pages_wanted].len = curr_len-pages_wanted;
	}
	p = cheri_setbounds(pool+page*pagesz, length);
	goto ok;

 ok:
	p = cheri_andperm(p, perms);
	//CHERI_PRINT_CAP(p);
	return p;

 fail:
	printf(KRED "mmap fail %lx\n", length);
	return MAP_FAILED;
}

static size_t addr2page(void * addr) {
	size_t page = (((char *)addr) - pool) / pagesz;
	assert((size_t)addr == (size_t)(pool + page*pagesz));
	return page;
}

static size_t addr2chunk(void * addr, size_t length) {
	size_t page = addr2page(addr);
	assert(length == pagesz*book[page].len);
	return page;
}

int __munmap(void *addr, size_t length) {
	if(!(cheri_getperm(addr) & CHERI_PERM_SOFT_1)) {
		errno = EINVAL;
		printf(KRED"BAD MUNMAP\n");
		return -1;
	}

	bzero(addr, length); /* clear mem */

	length += pagesz; /* fixme: fix for dlmalloc, see above */
	size_t page = addr2chunk(addr, length);

	book[page].status = page_released;
	release(addr);
	return 0;
}

void mfree(void *addr) {
	//CHERI_PRINT_CAP(addr);
	size_t page = addr2page(addr);
	book[page].status = page_unused;
}

void minit(capability all_mem) {


	size_t page_align = pagesz - 1;
	size_t length = cheri_getlen(all_mem);

	pages_nb = (length) / (pagesz + sizeof(page_t));
	assert(pages_nb > 0);
	size_t pool_len = pages_nb*pagesz;
	pool = cheri_setbounds(all_mem, pool_len);

	size_t book_len = pages_nb*sizeof(page_t);

	assert(book_len + pool_len <= length);
	book = cheri_setbounds(all_mem + pool_len, book_len);

	book[0].status = page_unused;
	book[0].len = pages_nb;
}
