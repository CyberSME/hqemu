// Structure layout of cpu registers the the bios uses.
//
// Copyright (C) 2008  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#ifndef __BREGS_H
#define __BREGS_H

// CPU flag bitdefs
#define F_CF (1<<0)
#define F_ZF (1<<6)
#define F_IF (1<<9)

// CR0 flags
#define CR0_PG (1<<31) // Paging
#define CR0_CD (1<<30) // Cache disable
#define CR0_NW (1<<29) // Not Write-through
#define CR0_PE (1<<0)  // Protection enable


#ifndef __ASSEMBLY__

#include "farptr.h" // struct segoff_s

/****************************************************************
 * Registers saved/restored in romlayout.S
 ****************************************************************/

#include "types.h" // u16

#define UREG(ER, R, RH, RL) union { u32 ER; struct { u16 R; u16 R ## _hi; }; struct { u8 RL; u8 RH; u8 R ## _hilo; u8 R ## _hihi; }; }

// Layout of registers passed in to irq handlers.  Note that this
// layout corresponds to code in romlayout.S - don't change it here
// without also updating the assembler code.
struct bregs {
    u16 ds;
    u16 es;
    UREG(edi, di, di_hi, di_lo);
    UREG(esi, si, si_hi, si_lo);
    UREG(ebp, bp, bp_hi, bp_lo);
    UREG(ebx, bx, bh, bl);
    UREG(edx, dx, dh, dl);
    UREG(ecx, cx, ch, cl);
    UREG(eax, ax, ah, al);
    struct segoff_s code;
    u16 flags;
} PACKED;


/****************************************************************
 * Helper functions
 ****************************************************************/

static inline void
set_cf(struct bregs *regs, int cond)
{
    if (cond)
        regs->flags |= F_CF;
    else
        regs->flags &= ~F_CF;
}

// Frequently used return codes
#define RET_EUNSUPPORTED 0x86

static inline void
set_success(struct bregs *regs)
{
    set_cf(regs, 0);
}

static inline void
set_code_success(struct bregs *regs)
{
    regs->ah = 0;
    set_cf(regs, 0);
}

static inline void
set_invalid_silent(struct bregs *regs)
{
    set_cf(regs, 1);
}

static inline void
set_code_invalid_silent(struct bregs *regs, u8 code)
{
    regs->ah = code;
    set_cf(regs, 1);
}

#endif // !__ASSEMBLY__

#endif // bregs.h
