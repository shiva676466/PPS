#pragma once
#ifndef RESTAURANT_STRUCT_H
#define RESTAURANT_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MENU_ITEMS    200
#define MAX_ORDERS        50
#define MAX_ORDER_ITEMS   20
#define MAX_NAME_LENGTH   50
#define TAX_RATE          0.08f
#define SERVICE_CHARGE    0.10f

typedef struct {
    int  id;
    char name[MAX_NAME_LENGTH];
    float price;
    char category[MAX_NAME_LENGTH];
    int  preparation_time;   // in minutes
} menu_item;

typedef struct {
    int   menu_item_id;
    int   quantity;
    float subtotal;
} order_item;

typedef struct {
    int     order_id;
    int     table_number;
    char    order_type[20];  // "dine-in", "takeaway"
    time_t  order_time;

    order_item items[MAX_ORDER_ITEMS];
    int        item_count;

    char   status[20];       // "pending", "preparing", "ready", "served", "cancelled"
    float  total_amount;
    float  tax;
    float  service_charge;
    float  final_total;
} order;

typedef struct {
    menu_item menu_items[MAX_MENU_ITEMS];
    int       menu_count;

    order orders[MAX_ORDERS];
    int   order_count;
    int   next_order_id;

    order* kitchen_queue[MAX_ORDERS];
    int    kitchen_front;
    int    kitchen_rear;

    float daily_revenue;
    int   order_frequency[24];   // orders per hour
} Restaurant;

#endif