/*-
 * Copyright (c) 2017 Lawrence Esswood
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
#ifndef CHERIOS_MSG_H
#define CHERIOS_MSG_H

#include "ccall.h"
#include "syscalls.h"

//TODO We have a slightly different ABI than libcheri, we don't use an argument number
//TODO Our ccall is more analogous to function pointers than objects, but we consider a function pointer to have
//TODO both a code and data componant.

//Leftmost arguments are for the message recipient. Other arguments are to the message enqueue function

// We define pretty much the same function in syscall.h, but we do it here again with a capability return type
// This is because message send really returns BOTH c3 and v0/v1.

__attribute__((cheri_ccall))
__attribute__((cheri_method_suffix("_inst")))
__attribute__((cheri_method_class(message_send_default_obj)))
capability message_send_c(register_t a0, register_t a1, register_t a2, register_t a3,
                          capability c3, capability c4, capability c5, capability c6,
                          act_kt dest, ccall_selector_t selector, register_t v0);


#endif //CHERIOS_MSG_H