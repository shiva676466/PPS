#pragma once
#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MENU_ITEMS 200
#define MAX_ORDERS 50
#define MAX_ORDER_ITEMS 20
#define MAX_NAME_LENGTH 50
#define TAX_RATE 0.08
#define SERVICE_CHARGE 0.10

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    char category[MAX_NAME_LENGTH];
    int preparation_time;
} menu_item;

typedef struct {
    int menu_item_id;
    int quantity;
    float subtotal;
} order_item;

typedef struct {
    int order_id;
    int table_number;
    char order_type[20];
    time_t order_time;
    order_item items[MAX_ORDER_ITEMS];
    int item_count;
    char status[20];
    float total_amount;
    float tax;
    float service_charge;
    float final_total;
} order;

typedef struct {
    menu_item menu_items[MAX_MENU_ITEMS];
    int menu_count;

    order orders[MAX_ORDERS];
    int order_count;
    int next_order_id;

    order* kitchen_queue[MAX_ORDERS];
    int kitchen_front;
    int kitchen_rear;

    float daily_revenue;
    int order_frequency[24];

} Restaurant;

/* Utility functions */
int find_menu_item_by_id(Restaurant* rest, int id);
int find_order_by_id(Restaurant* rest, int id);
void get_current_time_string(char* buffer);

#endif