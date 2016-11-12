/*-
 * Copyright (c) 2016 Robert N. M. Watson
 * Copyright (c) 2016 Hadrien Barral
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
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

#ifndef _BOOT_H_
#define _BOOT_H_

#include "mips.h"
#include "cdefs.h"
#include "stdio.h"

extern void	kernel_trampoline;
extern void	kernel_trampoline_end;

extern void	__boot_load_virtaddr;
extern void	__kernel_load_virtaddr;
extern void	__kernel_entry_point;

//fixme
#define	kernel_assert(e)	((e) ? (void)0 : __kernel_assert(__func__, \
				__FILE__, __LINE__, #e))
void	__kernel_assert(const char *, const char *, int, const char *) __dead2;
void	kernel_panic(const char *fmt, ...) __dead2;
#define printf kernel_printf
int	kernel_printf(const char *fmt, ...);
void	hw_reboot(void) __dead2;
int	kernel_vprintf(const char *fmt, va_list ap);

int	boot_printf(const char *fmt, ...);
int	boot_vprintf(const char *fmt, va_list ap);
void	boot_printf_syscall_enable(void);

void *	elf_loader(const char * s, void * (*alloc)(size_t size), void (*free)(void *addr), size_t * maxaddr);
void *	elf_loader_mem(void * addr, void * (*alloc)(size_t size), void (*free)(void *addr), size_t *maxaddr);
void *	load(const char * filename, int * len);

void	load_kernel();

void	hw_init(void);
void	install_exception_vector(void);
void	caches_invalidate(void * addr, size_t size);

#endif