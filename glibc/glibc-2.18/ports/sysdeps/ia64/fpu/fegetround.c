/* Return current rounding direction.
   Copyright (C) 1999-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Christian Boissat <Christian.Boissat@cern.ch>, 1999.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <get-rounding-mode.h>

int
fegetround (void)
{
  return get_rounding_mode ();
}
