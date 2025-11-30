#include "kitchen.h"
#include "orders.h"
#include <string.h>

int is_kitchen_queue_empty(Restaurant* rest) {
    return (rest->kitchen_rear + 1) % MAX_ORDERS == rest->kitchen_front;
}

int is_kitchen_queue_full(Restaurant* rest) {
    return (rest->kitchen_rear + 2) % MAX_ORDERS == rest->kitchen_front;
}

void enqueue_kitchen(Restaurant* rest, order* ord) {
    if (is_kitchen_queue_full(rest)) {
        printf("Kitchen queue is full!\n");
        return;
    }

    rest->kitchen_rear = (rest->kitchen_rear + 1) % MAX_ORDERS;
    rest->kitchen_queue[rest->kitchen_rear] = ord;
}

order* dequeue_kitchen(Restaurant* rest) {
    if (is_kitchen_queue_empty(rest)) {
        return NULL;
    }

    order* ord = rest->kitchen_queue[rest->kitchen_front];
    rest->kitchen_front = (rest->kitchen_front + 1) % MAX_ORDERS;
    return ord;
}

void process_kitchen_queue(Restaurant* rest) {
    printf("\n=== KITCHEN QUEUE PROCESSING ===\n");

    if (is_kitchen_queue_empty(rest)) {
        printf("No orders in kitchen queue.\n");
        return;
    }

    while (!is_kitchen_queue_empty(rest)) {
        order* ord = dequeue_kitchen(rest);
        if (ord && strcmp(ord->status, "cancelled") != 0) {
            printf("Processing Order %d (Table %d) - Status: %s\n",
                   ord->order_id, ord->table_number, ord->status);

            if (strcmp(ord->status, "pending") == 0) {
                update_order_status(rest, ord->order_id, "preparing");
                printf("Order %d is now being prepared.\n", ord->order_id);
            } else if (strcmp(ord->status, "preparing") == 0) {
                update_order_status(rest, ord->order_id, "ready");
                printf("Order %d is ready for serving.\n", ord->order_id);
            }
        }
    }
}