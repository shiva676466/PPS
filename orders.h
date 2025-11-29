#pragma once
#ifndef ORDERS_H
#define ORDERS_H

#include "restaurant_struct.h"

void initialize_restaurant(Restaurant* rest);

int  find_order_by_id(Restaurant* rest, int order_id);

int  place_order(Restaurant* rest, int table_num, char* order_type);
void add_item_to_order(Restaurant* rest, int order_id, int menu_item_id, int quantity);

void update_order_status(Restaurant* rest, int order_id, char* new_status);
void cancel_order(Restaurant* rest, int order_id);

void calculate_bill(Restaurant* rest, int order_id);
void print_invoice(Restaurant* rest, int order_id);

#endif