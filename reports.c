#include "reports.h"
#include <string.h>

void generate_reports(Restaurant* rest) {
    printf("\n=== RESTAURANT REPORTS ===\n");

    // daily revenue
    printf("Daily Revenue: $%.2f\n", rest->daily_revenue);

    // busiest hours
    printf("\nBusiest Hours Analysis:\n");
    int max_orders = 0;
    for (int i = 0; i < 24; i++) {
        if (rest->order_frequency[i] > max_orders) {
            max_orders = rest->order_frequency[i];
        }
    }

    if (max_orders == 0) {
        printf("No orders placed yet.\n");
    } else {
        for (int i = 0; i < 24; i++) {
            if (rest->order_frequency[i] == max_orders) {
                printf("Peak hour: %02d:00 with %d orders\n", i, max_orders);
            }
        }
    }

    // most ordered items
    int item_frequency[MAX_MENU_ITEMS] = {0};

    for (int i = 0; i < rest->order_count; i++) {
        order* ord = &rest->orders[i];
        for (int j = 0; j < ord->item_count; j++) {
            order_item* it = &ord->items[j];
            int idx = it->menu_item_id - 1;
            if (idx >= 0 && idx < MAX_MENU_ITEMS) {
                item_frequency[idx] += it->quantity;
            }
        }
    }

    // simple bubble sort for top items (by reordering a temporary index array)
    int index[MAX_MENU_ITEMS];
    for (int i = 0; i < MAX_MENU_ITEMS; i++) index[i] = i;

    for (int i = 0; i < rest->menu_count - 1; i++) {
        for (int j = 0; j < rest->menu_count - i - 1; j++) {
            if (item_frequency[index[j]] < item_frequency[index[j + 1]]) {
                int tmp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = tmp;
            }
        }
    }

    printf("\nTop 3 most popular items:\n");
    int printed = 0;
    for (int k = 0; k < rest->menu_count && printed < 3; k++) {
        int idx = index[k];
        if (item_frequency[idx] > 0) {
            menu_item* m = &rest->menu_items[idx];
            printf("%d. %s - Ordered %d times\n",
                   printed + 1, m->name, item_frequency[idx]);
            printed++;
        }
    }
    if (printed == 0) {
        printf("No items ordered yet.\n");
    }
}