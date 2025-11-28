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

// Structure definitions
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    char category[MAX_NAME_LENGTH];
    int preparation_time; // in minutes
} menu_item;

typedef struct {
    int menu_item_id;
    int quantity;
    float subtotal;
} order_item;

typedef struct {
    int order_id;
    int table_number;
    char order_type[20]; // "dine-in", "takeaway"
    time_t order_time;
    order_item items[MAX_ORDER_ITEMS];
    int item_count;
    char status[20]; // "pending", "preparing", "ready", "served", "cancelled"
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
    
    // Kitchen queue pointers
    order* kitchen_queue[MAX_ORDERS];
    int kitchen_front;
    int kitchen_rear;
    
    // Statistics
    float daily_revenue;
    int order_frequency[24]; // orders per hour
} Restaurant;

// Function prototypes
void initialize_restaurant(Restaurant* rest);
void add_menu_item(Restaurant* rest, int id, char* name, float price, char* category, int prep_time);
void display_menu(Restaurant* rest);
int place_order(Restaurant* rest, int table_num, char* order_type);
void add_item_to_order(Restaurant* rest, int order_id, int menu_item_id, int quantity);
void update_order_status(Restaurant* rest, int order_id, char* new_status);
void cancel_order(Restaurant* rest, int order_id);
void calculate_bill(Restaurant* rest, int order_id);
void print_invoice(Restaurant* rest, int order_id);
void process_kitchen_queue(Restaurant* rest);
void generate_reports(Restaurant* rest);
void bubble_sort_orders_by_time(order arr[], int n);
int binary_search_order(Restaurant* rest, int order_id);

// Queue operations
void enqueue_kitchen(Restaurant* rest, order* order);
order* dequeue_kitchen(Restaurant* rest);
int is_kitchen_queue_empty(Restaurant* rest);
int is_kitchen_queue_full(Restaurant* rest);

// Utility functions
int find_menu_item_by_id(Restaurant* rest, int id);
int find_order_by_id(Restaurant* rest, int id);
void get_current_time_string(char* buffer);

int main() {
    Restaurant my_restaurant;
    initialize_restaurant(&my_restaurant);
    
    // Sample menu items
    add_menu_item(&my_restaurant, 1, "Margherita Pizza", 12.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 2, "Caesar Salad", 8.99, "Appetizer", 10);
    add_menu_item(&my_restaurant, 3, "Grilled Salmon", 18.99, "Main Course", 20);
    add_menu_item(&my_restaurant, 4, "Chocolate Cake", 6.99, "Dessert", 5);
    add_menu_item(&my_restaurant, 5, "Iced Tea", 2.99, "Beverage", 2);
    add_menu_item(&my_restaurant, 6, "Garlic Bread", 4.99, "Appetizer", 8);
    
    // Add 100+ menu items
    add_menu_item(&my_restaurant, 7, "Pepperoni Pizza", 14.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 8, "Veggie Burger", 10.99, "Main Course", 12);
    add_menu_item(&my_restaurant, 9, "Chicken Wings", 11.99, "Appetizer", 15);
    add_menu_item(&my_restaurant, 10, "French Fries", 3.99, "Appetizer", 8);
    add_menu_item(&my_restaurant, 11, "Onion Rings", 4.99, "Appetizer", 10);
    add_menu_item(&my_restaurant, 12, "Mozzarella Sticks", 6.99, "Appetizer", 12);
    add_menu_item(&my_restaurant, 13, "Chicken Tenders", 9.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 14, "Fish and Chips", 13.99, "Main Course", 18);
    add_menu_item(&my_restaurant, 15, "BBQ Ribs", 16.99, "Main Course", 25);
    add_menu_item(&my_restaurant, 16, "Steak", 22.99, "Main Course", 20);
    add_menu_item(&my_restaurant, 17, "Grilled Chicken", 14.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 18, "Pasta Alfredo", 12.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 19, "Spaghetti Bolognese", 13.99, "Main Course", 17);
    add_menu_item(&my_restaurant, 20, "Lasagna", 14.99, "Main Course", 20);
    add_menu_item(&my_restaurant, 21, "Chicken Curry", 13.99, "Main Course", 18);
    add_menu_item(&my_restaurant, 22, "Vegetable Stir Fry", 11.99, "Main Course", 12);
    add_menu_item(&my_restaurant, 23, "Caesar Wrap", 8.99, "Main Course", 10);
    add_menu_item(&my_restaurant, 24, "Club Sandwich", 10.99, "Main Course", 12);
    add_menu_item(&my_restaurant, 25, "Greek Salad", 9.99, "Appetizer", 8);
    add_menu_item(&my_restaurant, 26, "Caprese Salad", 8.99, "Appetizer", 7);
    add_menu_item(&my_restaurant, 27, "Tomato Soup", 5.99, "Appetizer", 6);
    add_menu_item(&my_restaurant, 28, "Chicken Noodle Soup", 6.99, "Appetizer", 8);
    add_menu_item(&my_restaurant, 29, "Minestrone Soup", 5.99, "Appetizer", 7);
    add_menu_item(&my_restaurant, 30, "Garlic Shrimp", 12.99, "Appetizer", 15);
    add_menu_item(&my_restaurant, 31, "Calamari", 10.99, "Appetizer", 12);
    add_menu_item(&my_restaurant, 32, "Bruschetta", 7.99, "Appetizer", 8);
    add_menu_item(&my_restaurant, 33, "Nachos", 8.99, "Appetizer", 10);
    add_menu_item(&my_restaurant, 34, "Quesadilla", 9.99, "Appetizer", 12);
    add_menu_item(&my_restaurant, 35, "Guacamole", 6.99, "Appetizer", 5);
    add_menu_item(&my_restaurant, 36, "Salsa and Chips", 4.99, "Appetizer", 3);
    add_menu_item(&my_restaurant, 37, "Cheesecake", 7.99, "Dessert", 5);
    add_menu_item(&my_restaurant, 38, "Apple Pie", 6.99, "Dessert", 8);
    add_menu_item(&my_restaurant, 39, "Brownie Sundae", 8.99, "Dessert", 6);
    add_menu_item(&my_restaurant, 40, "Ice Cream", 4.99, "Dessert", 2);
    add_menu_item(&my_restaurant, 41, "Tiramisu", 8.99, "Dessert", 5);
    add_menu_item(&my_restaurant, 42, "Creme Brulee", 7.99, "Dessert", 6);
    add_menu_item(&my_restaurant, 43, "Chocolate Mousse", 6.99, "Dessert", 4);
    add_menu_item(&my_restaurant, 44, "Fruit Salad", 5.99, "Dessert", 5);
    add_menu_item(&my_restaurant, 45, "Lemon Tart", 6.99, "Dessert", 7);
    add_menu_item(&my_restaurant, 46, "Panna Cotta", 7.99, "Dessert", 5);
    add_menu_item(&my_restaurant, 47, "Coke", 2.49, "Beverage", 1);
    add_menu_item(&my_restaurant, 48, "Diet Coke", 2.49, "Beverage", 1);
    add_menu_item(&my_restaurant, 49, "Sprite", 2.49, "Beverage", 1);
    add_menu_item(&my_restaurant, 50, "Lemonade", 3.49, "Beverage", 2);
    add_menu_item(&my_restaurant, 51, "Orange Juice", 3.99, "Beverage", 2);
    add_menu_item(&my_restaurant, 52, "Apple Juice", 3.99, "Beverage", 2);
    add_menu_item(&my_restaurant, 53, "Coffee", 2.99, "Beverage", 3);
    add_menu_item(&my_restaurant, 54, "Tea", 2.49, "Beverage", 3);
    add_menu_item(&my_restaurant, 55, "Hot Chocolate", 3.99, "Beverage", 4);
    add_menu_item(&my_restaurant, 56, "Milk", 2.99, "Beverage", 1);
    add_menu_item(&my_restaurant, 57, "Water", 1.99, "Beverage", 1);
    add_menu_item(&my_restaurant, 58, "Beer", 5.99, "Beverage", 2);
    add_menu_item(&my_restaurant, 59, "Wine", 7.99, "Beverage", 2);
    add_menu_item(&my_restaurant, 60, "Margarita", 8.99, "Beverage", 5);
    add_menu_item(&my_restaurant, 61, "Mojito", 8.49, "Beverage", 5);
    add_menu_item(&my_restaurant, 62, "BBQ Chicken Pizza", 15.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 63, "Hawaiian Pizza", 14.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 64, "Vegetarian Pizza", 13.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 65, "Supreme Pizza", 16.99, "Main Course", 18);
    add_menu_item(&my_restaurant, 66, "Buffalo Chicken Pizza", 15.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 67, "Meat Lovers Pizza", 17.99, "Main Course", 19);
    add_menu_item(&my_restaurant, 68, "White Pizza", 14.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 69, "Calzone", 12.99, "Main Course", 17);
    add_menu_item(&my_restaurant, 70, "Garlic Knots", 5.99, "Appetizer", 8);
    add_menu_item(&my_restaurant, 71, "Bread Sticks", 4.99, "Appetizer", 6);
    add_menu_item(&my_restaurant, 72, "Stuffed Mushrooms", 8.99, "Appetizer", 12);
    add_menu_item(&my_restaurant, 73, "Spinach Artichoke Dip", 9.99, "Appetizer", 10);
    add_menu_item(&my_restaurant, 74, "Potato Skins", 7.99, "Appetizer", 12);
    add_menu_item(&my_restaurant, 75, "Chicken Quesadilla", 10.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 76, "Beef Burrito", 11.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 77, "Chicken Tacos", 9.99, "Main Course", 12);
    add_menu_item(&my_restaurant, 78, "Fish Tacos", 10.99, "Main Course", 13);
    add_menu_item(&my_restaurant, 79, "Vegetable Tacos", 8.99, "Main Course", 10);
    add_menu_item(&my_restaurant, 80, "Chicken Burrito Bowl", 12.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 81, "Steak Fajitas", 16.99, "Main Course", 18);
    add_menu_item(&my_restaurant, 82, "Chicken Fajitas", 14.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 83, "Shrimp Scampi", 17.99, "Main Course", 20);
    add_menu_item(&my_restaurant, 84, "Lobster Roll", 19.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 85, "Crab Cakes", 15.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 86, "Fish Burger", 11.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 87, "Veggie Wrap", 9.99, "Main Course", 10);
    add_menu_item(&my_restaurant, 88, "Turkey Club", 11.99, "Main Course", 12);
    add_menu_item(&my_restaurant, 89, "Reuben Sandwich", 12.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 90, "Philly Cheesesteak", 13.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 91, "Chicken Parmesan", 15.99, "Main Course", 18);
    add_menu_item(&my_restaurant, 92, "Eggplant Parmesan", 13.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 93, "Meatballs", 12.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 94, "Chicken Fried Rice", 11.99, "Main Course", 14);
    add_menu_item(&my_restaurant, 95, "Beef Fried Rice", 12.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 96, "Vegetable Fried Rice", 10.99, "Main Course", 12);
    add_menu_item(&my_restaurant, 97, "Chicken Lo Mein", 12.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 98, "Beef Lo Mein", 13.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 99, "Vegetable Lo Mein", 11.99, "Main Course", 13);
    add_menu_item(&my_restaurant, 100, "Kung Pao Chicken", 14.99, "Main Course", 17);
    add_menu_item(&my_restaurant, 101, "Sweet and Sour Pork", 13.99, "Main Course", 16);
    add_menu_item(&my_restaurant, 102, "Mongolian Beef", 15.99, "Main Course", 18);
    add_menu_item(&my_restaurant, 103, "General Tso Chicken", 14.99, "Main Course", 17);
    add_menu_item(&my_restaurant, 104, "Orange Chicken", 14.99, "Main Course", 17);
    add_menu_item(&my_restaurant, 105, "Beef with Broccoli", 13.99, "Main Course", 15);
    add_menu_item(&my_restaurant, 106, "Chicken with Broccoli", 12.99, "Main Course", 14);
    
    int choice, order_id, table_num, menu_id, quantity;
    char order_type[20];
    
    printf("=== Restaurant order Management System ===\n");
    
    while(1) {
        printf("\n1. Display Menu\n");
        printf("2. Place New order\n");
        printf("3. Add Item to Existing order\n");
        printf("4. Update order Status\n");
        printf("5. Cancel order\n");
        printf("6. Calculate Bill\n");
        printf("7. Print Invoice\n");
        printf("8. Process Kitchen Queue\n");
        printf("9. Generate Reports\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                display_menu(&my_restaurant);
                break;
                
            case 2:
                printf("Enter table number: ");
                scanf("%d", &table_num);
                printf("Enter order type (dine-in/takeaway): ");
                scanf("%s", order_type);
                order_id = place_order(&my_restaurant, table_num, order_type);
                if(order_id != -1) {
                    printf("order created successfully! order ID: %d\n", order_id);
                }
                break;
                
            case 3:
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                printf("Enter menu item ID: ");
                scanf("%d", &menu_id);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                add_item_to_order(&my_restaurant, order_id, menu_id, quantity);
                break;
                
            case 4:
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                printf("Enter new status (pending/preparing/ready/served/cancelled): ");
                scanf("%s", order_type);
                update_order_status(&my_restaurant, order_id, order_type);
                break;
                
            case 5:
                printf("Enter order ID to cancel: ");
                scanf("%d", &order_id);
                cancel_order(&my_restaurant, order_id);
                break;
                
            case 6:
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                calculate_bill(&my_restaurant, order_id);
                break;
                
            case 7:
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                print_invoice(&my_restaurant, order_id);
                break;
                
            case 8:
                process_kitchen_queue(&my_restaurant);
                break;
                
            case 9:
                generate_reports(&my_restaurant);
                break;
                
            case 10:
                printf("Thank you for using the system!\n");
                exit(0);
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

void initialize_restaurant(Restaurant* rest) {
    rest->menu_count = 0;
    rest->order_count = 0;
    rest->next_order_id = 1001;
    rest->kitchen_front = 0;
    rest->kitchen_rear = -1;
    rest->daily_revenue = 0.0;
    
    for(int i = 0; i < 24; i++) {
        rest->order_frequency[i] = 0;
    }
}

void add_menu_item(Restaurant* rest, int id, char* name, float price, char* category, int prep_time) {
    if(rest->menu_count >= MAX_MENU_ITEMS) {
        printf("Menu is full! Cannot add more items.\n");
        return;
    }
    
    menu_item* item = &rest->menu_items[rest->menu_count];
    item->id = id;
    strcpy(item->name, name);
    item->price = price;
    strcpy(item->category, category);
    item->preparation_time = prep_time;
    
    rest->menu_count++;
    printf("Menu item '%s' added successfully!\n", name);
}

void display_menu(Restaurant* rest) {
    printf("\n=== RESTAURANT MENU ===\n");
    printf("%-4s %-20s %-15s %-10s %s\n", "ID", "Name", "Category", "Price", "Prep Time");
    printf("------------------------------------------------------------------------\n");
    
    for(int i = 0; i < rest->menu_count; i++) {
        menu_item* item = &rest->menu_items[i];
        printf("%-4d %-20s %-15s $%-9.2f %d mins\n", 
               item->id, item->name, item->category, item->price, item->preparation_time);
    }
}

int place_order(Restaurant* rest, int table_num, char* order_type) {
    if(rest->order_count >= MAX_ORDERS) {
        printf("Cannot accept more orders! Kitchen is at full capacity.\n");
        return -1;
    }
    
    order* new_order = &rest->orders[rest->order_count];
    new_order->order_id = rest->next_order_id++;
    new_order->table_number = table_num;
    strcpy(new_order->order_type, order_type);
    new_order->order_time = time(NULL);
    new_order->item_count = 0;
    strcpy(new_order->status, "pending");
    new_order->total_amount = 0.0;
    
    // Record order hour for frequency analysis
    struct tm* timeinfo = localtime(&new_order->order_time);
    rest->order_frequency[timeinfo->tm_hour]++;
    
    rest->order_count++;
    
    // Add items to order
    int add_more = 1;
    while(add_more && new_order->item_count < MAX_ORDER_ITEMS) {
        int menu_id, quantity;
        printf("Enter menu item ID (0 to stop adding): ");
        scanf("%d", &menu_id);
        
        if(menu_id == 0) break;
        
        printf("Enter quantity: ");
        scanf("%d", &quantity);
        
        add_item_to_order(rest, new_order->order_id, menu_id, quantity);
        
        printf("Add another item? (1=Yes, 0=No): ");
        scanf("%d", &add_more);
    }
    
    // Add to kitchen queue
    enqueue_kitchen(rest, new_order);
    
    return new_order->order_id;
}

void add_item_to_order(Restaurant* rest, int order_id, int menu_item_id, int quantity) {
    int order_index = find_order_by_id(rest, order_id);
    if(order_index == -1) {
        printf("order not found!\n");
        return;
    }
    
    order* order = &rest->orders[order_index];
    
    if(order->item_count >= MAX_ORDER_ITEMS) {
        printf("order is full! Cannot add more items.\n");
        return;
    }
    
    int menu_index = find_menu_item_by_id(rest, menu_item_id);
    if(menu_index == -1) {
        printf("Menu item not found!\n");
        return;
    }
    
    menu_item* menu_item = &rest->menu_items[menu_index];
    
    order_item* order_item = &order->items[order->item_count];
    order_item->menu_item_id = menu_item_id;
    order_item->quantity = quantity;
    order_item->subtotal = menu_item->price * quantity;
    
    order->total_amount += order_item->subtotal;
    order->item_count++;
    
    printf("Added %d x %s to order %d\n", quantity, menu_item->name, order_id);
}

void update_order_status(Restaurant* rest, int order_id, char* new_status) {
    int order_index = find_order_by_id(rest, order_id);
    if(order_index == -1) {
        printf("order not found!\n");
        return;
    }
    
    order* order = &rest->orders[order_index];
    strcpy(order->status, new_status);
    
    printf("order %d status updated to: %s\n", order_id, new_status);
    
    // If order is served, calculate final bill and update revenue
    if(strcmp(new_status, "served") == 0) {
        calculate_bill(rest, order_id);
        rest->daily_revenue += order->final_total;
    }
}

void cancel_order(Restaurant* rest, int order_id) {
    int order_index = find_order_by_id(rest, order_id);
    if(order_index == -1) {
        printf("order not found!\n");
        return;
    }
    
    order* order = &rest->orders[order_index];
    
    if(strcmp(order->status, "served") == 0) {
        printf("Cannot cancel served order!\n");
        return;
    }
    
    strcpy(order->status, "cancelled");
    printf("order %d has been cancelled.\n", order_id);
    
    // Remove from kitchen queue if present
    // This is a simplified approach - in real scenario would need proper queue removal
}

void calculate_bill(Restaurant* rest, int order_id) {
    int order_index = find_order_by_id(rest, order_id);
    if(order_index == -1) {
        printf("order not found!\n");
        return;
    }
    
    order* order = &rest->orders[order_index];
    
    order->tax = order->total_amount * TAX_RATE;
    order->service_charge = order->total_amount * SERVICE_CHARGE;
    order->final_total = order->total_amount + order->tax + order->service_charge;
    
    printf("Bill calculated for order %d. Total: $%.2f\n", order_id, order->final_total);
}

void print_invoice(Restaurant* rest, int order_id) {
    int order_index = find_order_by_id(rest, order_id);
    if(order_index == -1) {
        printf("order not found!\n");
        return;
    }
    
    order* order = &rest->orders[order_index];
    
    printf("\n=== INVOICE ===\n");
    printf("order ID: %d\n", order->order_id);
    printf("Table: %d\n", order->table_number);
    printf("Type: %s\n", order->order_type);
    
    char time_str[30];
    get_current_time_string(time_str);
    printf("order Time: %s", ctime(&order->order_time));
    printf("Status: %s\n", order->status);
    
    printf("\n%-20s %-8s %-10s %s\n", "Item", "Qty", "Price", "Subtotal");
    printf("--------------------------------------------------------\n");
    
    for(int i = 0; i < order->item_count; i++) {
        order_item* item = &order->items[i];
        int menu_index = find_menu_item_by_id(rest, item->menu_item_id);
        if(menu_index != -1) {
            menu_item* menu_item = &rest->menu_items[menu_index];
            printf("%-20s %-8d $%-9.2f $%-8.2f\n", 
                   menu_item->name, item->quantity, menu_item->price, item->subtotal);
        }
    }
    
    printf("\nSubtotal: $%.2f\n", order->total_amount);
    printf("Tax (8%%): $%.2f\n", order->tax);
    printf("Service Charge (10%%): $%.2f\n", order->service_charge);
    printf("TOTAL: $%.2f\n", order->final_total);
    printf("====================\n");
}

void process_kitchen_queue(Restaurant* rest) {
    printf("\n=== KITCHEN QUEUE PROCESSING ===\n");
    
    if(is_kitchen_queue_empty(rest)) {
        printf("No orders in kitchen queue.\n");
        return;
    }
    
    // Process orders in queue
    while(!is_kitchen_queue_empty(rest)) {
        order* order = dequeue_kitchen(rest);
        if(order && strcmp(order->status, "cancelled") != 0) {
            printf("Processing order %d (Table %d) - Status: %s\n", 
                   order->order_id, order->table_number, order->status);
            
            // Simulate kitchen processing
            if(strcmp(order->status, "pending") == 0) {
                update_order_status(rest, order->order_id, "preparing");
                printf("order %d is now being prepared.\n", order->order_id);
            } else if(strcmp(order->status, "preparing") == 0) {
                update_order_status(rest, order->order_id, "ready");
                printf("order %d is ready for serving.\n", order->order_id);
            }
        }
    }
}

void generate_reports(Restaurant* rest) {
    printf("\n=== RESTAURANT REPORTS ===\n");
    
    // Daily Revenue
    printf("Daily Revenue: $%.2f\n", rest->daily_revenue);
    
    // Busiest Hours
    printf("\nBusiest Hours Analysis:\n");
    int max_orders = 0;
    for(int i = 0; i < 24; i++) {
        if(rest->order_frequency[i] > max_orders) {
            max_orders = rest->order_frequency[i];
        }
    }
    
    for(int i = 0; i < 24; i++) {
        if(rest->order_frequency[i] == max_orders && max_orders > 0) {
            printf("Peak hour: %02d:00 with %d orders\n", i, max_orders);
        }
    }
    
    // Most ordered items
    printf("\nMost Ordered Items:\n");
    int item_frequency[MAX_MENU_ITEMS] = {0};
    
    for(int i = 0; i < rest->order_count; i++) {
        order* order = &rest->orders[i];
        for(int j = 0; j < order->item_count; j++) {
            order_item* item = &order->items[j];
            item_frequency[item->menu_item_id - 1] += item->quantity;
        }
    }
    
    // Simple bubble sort to find top items
    for(int i = 0; i < rest->menu_count - 1; i++) {
        for(int j = 0; j < rest->menu_count - i - 1; j++) {
            if(item_frequency[j] < item_frequency[j + 1]) {
                // Swap frequencies
                int temp_freq = item_frequency[j];
                item_frequency[j] = item_frequency[j + 1];
                item_frequency[j + 1] = temp_freq;
                
                // Swap menu items
                menu_item temp_item = rest->menu_items[j];
                rest->menu_items[j] = rest->menu_items[j + 1];
                rest->menu_items[j + 1] = temp_item;
            }
        }
    }
    
    printf("Top 3 most popular items:\n");
    for(int i = 0; i < 3 && i < rest->menu_count; i++) {
        if(item_frequency[i] > 0) {
            printf("%d. %s - Ordered %d times\n", 
                   i + 1, rest->menu_items[i].name, item_frequency[i]);
        }
    }
}

// Queue Implementation
void enqueue_kitchen(Restaurant* rest, order* order) {
    if(is_kitchen_queue_full(rest)) {
        printf("Kitchen queue is full!\n");
        return;
    }
    
    rest->kitchen_rear = (rest->kitchen_rear + 1) % MAX_ORDERS;
    rest->kitchen_queue[rest->kitchen_rear] = order;
}

order* dequeue_kitchen(Restaurant* rest) {
    if(is_kitchen_queue_empty(rest)) {
        return NULL;
    }
    
    order* order = rest->kitchen_queue[rest->kitchen_front];
    rest->kitchen_front = (rest->kitchen_front + 1) % MAX_ORDERS;
    return order;
}

int is_kitchen_queue_empty(Restaurant* rest) {
    return (rest->kitchen_rear + 1) % MAX_ORDERS == rest->kitchen_front;
}

int is_kitchen_queue_full(Restaurant* rest) {
    return (rest->kitchen_rear + 2) % MAX_ORDERS == rest->kitchen_front;
}

// Utility Functions
int find_menu_item_by_id(Restaurant* rest, int id) {
    for(int i = 0; i < rest->menu_count; i++) {
        if(rest->menu_items[i].id == id) {
            return i;
        }
    }
    return -1;
}

int find_order_by_id(Restaurant* rest, int order_id) {
    for(int i = 0; i < rest->order_count; i++) {
        if(rest->orders[i].order_id == order_id) {
            return i;
        }
    }
    return -1;
}

void get_current_time_string(char* buffer) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Sorting and Searching (for future enhancements)
void bubble_sort_orders_by_time(order arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j].order_time > arr[j+1].order_time) {
                order temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int binary_search_order(Restaurant* rest, int order_id) {
    int left = 0;
    int right = rest->order_count - 1;
    
    while(left <= right) {
        int mid = left + (right - left) / 2;
        
        if(rest->orders[mid].order_id == order_id) {
            return mid;
        }
        
        if(rest->orders[mid].order_id < order_id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}