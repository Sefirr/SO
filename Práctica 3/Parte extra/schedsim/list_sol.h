/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_LIST_SOL_H
#define	_LIST_SOL_H

//#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include "list_sol_impl.h"

#ifndef offsetof
#define offsetof(type, member)	((size_t)(&((type *)0)->member))
#endif

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct list_sol_node list_sol_node_t;
typedef struct list_sol list_sol_t;

void list_sol_create(list_sol_t *, size_t, size_t);
void list_sol_destroy(list_sol_t *);

void list_sol_insert_after(list_sol_t *, void *, void *);
void list_sol_insert_before(list_sol_t *, void *, void *);
void list_sol_insert_head(list_sol_t *, void *);
void list_sol_insert_tail(list_sol_t *, void *);
void list_sol_remove(list_sol_t *, void *);
void *list_sol_remove_head(list_sol_t *);
void *list_sol_remove_tail(list_sol_t *);
void list_sol_move_tail(list_sol_t *, list_sol_t *);

void *list_sol_head(list_sol_t *);
void *list_sol_tail(list_sol_t *);
void *list_sol_next(list_sol_t *, void *);
void *list_sol_prev(list_sol_t *, void *);
int list_sol_is_empty(list_sol_t *);

void list_sol_link_init(list_sol_node_t *);
void list_sol_link_replace(list_sol_node_t *, list_sol_node_t *);

int list_sol_link_active(list_sol_node_t *);

#ifdef	__cplusplus
}
#endif

#endif	/* _LIST_SOL_H */
