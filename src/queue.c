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
#include <stdlib.h>

#include "queue.h"

Queue *queue_new()
{
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

void queue_destroy(Queue *q)
{
    QueueElem *current = q->first;
    while (current != NULL)
    {
        QueueElem *next = current->next;
        free(current);
        current = next;
    }
    free(q);
}

void queue_enqueue(Queue *q, void *elem)
{
    QueueElem *qelem;

    qelem = (QueueElem*)malloc(sizeof(QueueElem));
    qelem->value = elem;
    qelem->next = NULL;

    if (q->last == NULL)
    {
        q->last = qelem;
        q->first = qelem;
    }
    else
    {
        q->last->next = qelem;
        q->last = qelem;
    }
}

void *queue_dequeue(Queue *q)
{
    QueueElem *first = q->first;
    if (first != NULL)
    {
        void *value = first->value;
        q->first = first->next;
        free(first);
        return value;
    }
    else
    {
        return NULL;
    }
}

