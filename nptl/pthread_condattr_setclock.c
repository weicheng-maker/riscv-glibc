/* Copyright (C) 2003-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2003.

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

#include <assert.h>
#include <errno.h>
#include <futex-internal.h>
#include <time.h>
#include <sysdep.h>
#include "pthreadP.h"


int
pthread_condattr_setclock (pthread_condattr_t *attr, clockid_t clock_id)
{
  /* Only a few clocks are allowed.  */
  if (clock_id != CLOCK_MONOTONIC && clock_id != CLOCK_REALTIME)
    /* If more clocks are allowed some day the storing of the clock ID
       in the pthread_cond_t structure needs to be adjusted.  */
    return EINVAL;

  /* If we do not support waiting using CLOCK_MONOTONIC, return an error.  */
  if (clock_id == CLOCK_MONOTONIC
      && !futex_supports_exact_relative_timeouts())
    return ENOTSUP;

  /* Make sure the value fits in the bits we reserved.  */
  assert (clock_id < (1 << COND_CLOCK_BITS));

  int *valuep = &((struct pthread_condattr *) attr)->value;

  *valuep = ((*valuep & ~(((1 << COND_CLOCK_BITS) - 1) << 1))
	     | (clock_id << 1));

  return 0;
}
