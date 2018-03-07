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
#ifndef CHERIOS_DYLINK_H
#define CHERIOS_DYLINK_H

#include "string_enums.h"
#include "cheric.h"

#define DOMAIN_TYPE_LIST(ITEM)  \
    ITEM(callable_taken, 0)     \
    ITEM(callable_ready, 1)     \
    ITEM(returnable_ready, 2)   \
    ITEM(returnable_used, 3)    \
    ITEM(reserved_start, 4)     \
    ITEM(reserved_end, 254)     \
    ITEM(user_type, 255)

DECLARE_ENUM(domain_type_t, DOMAIN_TYPE_LIST)

#define DEFINE_DOMAIN_T     DEFINE_ENUM_CASE(domain_type_t, DOMAIN_TYPE_LIST)

#define DOMAIN_TYPE_MASK    0xff
#define DOMAIN_INFO_SHIFT   8

// These offsets are required by the nano-kernel. An exception will SWAP pcc/idc and STORE c1
// An exception restore call will RESTORE pcc/idc/c1, and write the previous values of pcc and idc
#define CTLP_OFFSET_EX_PCC  (CAP_SIZE)
#define CTLP_OFFSET_EX_IDC  (CAP_SIZE * 2)
#define CTLP_OFFSET_EX_C1   (CAP_SIZE * 3)

// These are our own
#define CTLP_OFFSET_CSP  (CAP_SIZE*4)
#define CTLP_OFFSET_CUSP (CAP_SIZE*5)
#define CTLP_OFFSET_CDS  (CAP_SIZE*6)
#define CTLP_OFFSET_CDL  (CAP_SIZE*7)
#define CTLP_OFFSET_CGP  (CAP_SIZE*8)

// From unsafe stack base
#define CSP_OFF_NEXT    (-1 * CAP_SIZE)
// From unsafe stack end
#define CSP_OFF_PREV    (-2 * CAP_SIZE)

#ifndef __ASSEMBLY__

#include "stddef.h"

typedef struct guard_t {
    register_t guard; // of type (domain_type_t | (info << 8))
    char pad[CHERICAP_SIZE - sizeof(register_t)];
} guard_t;

typedef void entry_stub_t(void);

typedef void ex_pcc_t(void);

typedef struct CTL_t {
    guard_t guard;
    // On exception pcc/idc will do set to these.
    // All 3 will have the exceptional pcc/idc/c1 saved in them
    // On restore all 3 will be restored, and ex_pcc/_exidc will be what they originally were
    // BEFORE the exception (i.e. the handler is unchanged)
    // Note c1 is a SCRATCH register on entry to your exception routine
    ex_pcc_t* ex_pcc;
    capability ex_idc;
    capability ex_c1;
    capability csp;
    capability cusp;
    capability cds;
    entry_stub_t* cdl;
    capability cgp;
    capability captable[];
} CTL_t;

_Static_assert(offsetof(CTL_t, guard) == 0,             "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, ex_pcc) == CTLP_OFFSET_EX_PCC,  "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, ex_idc) == CTLP_OFFSET_EX_IDC,  "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, ex_c1) == CTLP_OFFSET_EX_C1,  "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, csp) == CTLP_OFFSET_CSP,  "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, cusp) == CTLP_OFFSET_CUSP, "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, cds) == CTLP_OFFSET_CDS,  "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, cdl) == CTLP_OFFSET_CDL,  "CGP offsets need to match assembly assumptions");
_Static_assert(offsetof(CTL_t, cgp) == CTLP_OFFSET_CGP,  "CGP offsets need to match assembly assumptions");

static inline CTL_t* get_ctl(void) {
    CTL_t* ret;
    __asm__ ("cmove %[ret], $idc" : [ret]"=C"(ret) ::);
    return ret;
}

static __inline__ capability get_cgp(void) {
    capability object;
    __asm__ (
    "cmove %[object], $c25 \n"
    : [object]"=C" (object));
    return object;
}
#endif // ASSEMBLY

#endif //CHERIOS_DYLINK_H