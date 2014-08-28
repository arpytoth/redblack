/* 
 * Copyright (C) 2014 Toth Arpad 
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *    
 *   Toth Arpad (arpytoth@yahoo.com)
 */
#ifndef _TOTH_QUEUE_H_
#define _TOTH_QUEUE_H_

typedef struct QueueElem
{
    void *value;
    struct QueueElem *next;
} QueueElem;

typedef struct Queue
{
    struct QueueElem *first;
    struct QueueElem *last;
} Queue;


Queue *queue_new();
void queue_destroy(Queue *q);
void queue_enqueue(Queue *q, void *elem);
void *queue_dequeue(Queue *q);
#endif // _TOTH_QUEUE_H_

