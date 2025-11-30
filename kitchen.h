#pragma once
#ifndef KITCHEN_H
#define KITCHEN_H

#include "restaurant_struct.h"

void enqueue_kitchen(Restaurant* rest, order* ord);
order* dequeue_kitchen(Restaurant* rest);
int   is_kitchen_queue_empty(Restaurant* rest);
int   is_kitchen_queue_full(Restaurant* rest);
void  process_kitchen_queue(Restaurant* rest);

#endif