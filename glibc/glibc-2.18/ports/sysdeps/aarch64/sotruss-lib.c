/* Override generic sotruss-lib.c to define actual functions for AArch64.
   Copyright (C) 2012-2013 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#define HAVE_ARCH_PLTENTER
#define HAVE_ARCH_PLTEXIT

#include <elf/sotruss-lib.c>

ElfW(Addr)
la_aarch64_gnu_pltenter (ElfW(Sym) *sym __attribute__ ((unused)),
			 unsigned int ndx __attribute__ ((unused)),
			 uintptr_t *refcook, uintptr_t *defcook,
			 La_aarch64_regs *regs, unsigned int *flags,
			 const char *symname, long int *framesizep)
{
  print_enter (refcook, defcook, symname,
	       regs->lr_xreg[0], regs->lr_xreg[1], regs->lr_xreg[2],
	       *flags);

  /* No need to copy anything, we will not need the parameters in any case.  */
  *framesizep = 0;

  return sym->st_value;
}

unsigned int
la_aarch64_gnu_pltexit (ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
			uintptr_t *defcook,
			const struct La_aarch64_regs *inregs,
			struct La_aarch64_retval *outregs, const char *symname)
{
  print_exit (refcook, defcook, symname, outregs->lrv_xreg[0]);

  return 0;
}
