// #include<stdio.h>

// struct employee 
// {
//     int id;
//     char name[30];
//     float salary;
//     char department[20];
// };

// int main(){
//     struct employee e1;

//     printf("Enter employee ID : \n");
//     scanf("%d", &e1.id);
//     printf("Enter employee name : \n");
//     scanf("%s", &e1.name);
//     printf("Enter employee salary : \n");
//     scanf("%f", &e1.salary);
//     printf("Enter department : \n");
//     fgets(e1.department, 20, stdin);
//     printf("\n========= Employee Detaiks =========\n");
//     printf("ID : %d\n", e1.id);
//     fgets(e1.name, 30, stdin);
//     printf("Salary : %f\n", e1.salary);
//     printf("Department : %s\n", e1.department);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FOOD_ITEMS 200
#define MAX_CUSTOMER_ORDERS 50
#define MAX_ITEMS_PER_ORDER 20
#define MAX_ITEM_NAME 50
#define SALES_TAX 0.08
#define SERVICE_FEE 0.10

// Data structures
typedef struct {
    int code;
    char item_name[MAX_ITEM_NAME];
    double cost;
    char type[MAX_ITEM_NAME];
    int cook_time;
} FoodItem;

typedef struct {
    int food_code;
    int amount;
    double item_total;
} OrderedProduct;

typedef struct {
    int transaction_id;
    int customer_table;
    char service_mode[15];
    time_t timestamp;
    OrderedProduct products[MAX_ITEMS_PER_ORDER];
    int product_count;
    char current_state[15];
    double subtotal_cost;
    double tax_amount;
    double service_fee;
    double grand_total;
} CustomerTransaction;

typedef struct {
    FoodItem food_menu[MAX_FOOD_ITEMS];
    int total_menu_items;
    
    CustomerTransaction all_orders[MAX_CUSTOMER_ORDERS];
    int active_orders;
    int next_transaction_id;
    
    // Cooking line management
    CustomerTransaction* cooking_line[MAX_CUSTOMER_ORDERS];
    int line_start;
    int line_end;
    
    // Analytics
    double total_earnings;
    int hourly_orders[24];
} FoodBusiness;

// Function declarations
void setupBusiness(FoodBusiness* biz);
void registerFoodItem(FoodBusiness* biz, int code, char* name, double price, char* type, int cook_duration);
void showFoodMenu(FoodBusiness* biz);
int createNewOrder(FoodBusiness* biz, int table_num, char* service_mode);
void includeProductInOrder(FoodBusiness* biz, int order_code, int product_code, int amount);
void modifyOrderState(FoodBusiness* biz, int order_code, char* new_state);
void removeOrder(FoodBusiness* biz, int order_code);
void computeOrderTotal(FoodBusiness* biz, int order_code);
void generateReceipt(FoodBusiness* biz, int order_code);
void handleCookingLine(FoodBusiness* biz);
void produceAnalytics(FoodBusiness* biz);
void sortOrdersByTimestamp(CustomerTransaction arr[], int n);
int locateOrderByCode(FoodBusiness* biz, int order_code);

// Cooking line operations
void addToCookingLine(FoodBusiness* biz, CustomerTransaction* transaction);
CustomerTransaction* removeFromCookingLine(FoodBusiness* biz);
int isCookingLineEmpty(FoodBusiness* biz);
int isCookingLineFull(FoodBusiness* biz);

// Helper functions
int findFoodItemByCode(FoodBusiness* biz, int code);
int findTransactionByCode(FoodBusiness* biz, int transaction_code);
void getFormattedTime(char* output);

int main() {
    FoodBusiness myBusiness;
    setupBusiness(&myBusiness);
    
    // Initialize food catalog
    registerFoodItem(&myBusiness, 1, "Classic Margherita", 12.50, "Entree", 15);
    registerFoodItem(&myBusiness, 2, "Fresh Caesar Salad", 9.25, "Starter", 10);
    registerFoodItem(&myBusiness, 3, "Atlantic Salmon Grill", 19.75, "Entree", 20);
    registerFoodItem(&myBusiness, 4, "Decadent Chocolate Cake", 7.25, "Sweet", 5);
    registerFoodItem(&myBusiness, 5, "Fresh Brewed Iced Tea", 3.25, "Drink", 2);
    registerFoodItem(&myBusiness, 6, "Herb Garlic Toast", 5.50, "Starter", 8);

    // Extended food items catalog
    registerFoodItem(&myBusiness, 7, "Spicy Pepperoni Pizza", 15.25, "Entree", 15);
    registerFoodItem(&myBusiness, 8, "Garden Veggie Burger", 11.75, "Entree", 12);
    registerFoodItem(&myBusiness, 9, "Crispy Chicken Wings", 12.50, "Starter", 15);
    registerFoodItem(&myBusiness, 10, "Golden French Fries", 4.25, "Starter", 8);
    registerFoodItem(&myBusiness, 11, "Crispy Onion Rings", 5.25, "Starter", 10);
    registerFoodItem(&myBusiness, 12, "Mozzarella Cheese Sticks", 7.25, "Starter", 12);
    registerFoodItem(&myBusiness, 13, "Crispy Chicken Strips", 10.50, "Entree", 14);
    registerFoodItem(&myBusiness, 14, "Traditional Fish & Chips", 14.25, "Entree", 18);
    registerFoodItem(&myBusiness, 15, "BBQ Glazed Ribs", 17.50, "Entree", 25);
    registerFoodItem(&myBusiness, 16, "Prime Beef Steak", 23.50, "Entree", 20);
    registerFoodItem(&myBusiness, 17, "Herb Grilled Chicken", 15.75, "Entree", 16);
    registerFoodItem(&myBusiness, 18, "Creamy Alfredo Pasta", 13.25, "Entree", 15);
    registerFoodItem(&myBusiness, 19, "Traditional Spaghetti", 14.50, "Entree", 17);
    registerFoodItem(&myBusiness, 20, "Layered Lasagna", 15.25, "Entree", 20);
    registerFoodItem(&myBusiness, 21, "Aromatic Chicken Curry", 14.25, "Entree", 18);
    registerFoodItem(&myBusiness, 22, "Asian Vegetable Stir Fry", 12.50, "Entree", 12);
    registerFoodItem(&myBusiness, 23, "Caesar Chicken Wrap", 9.25, "Entree", 10);
    registerFoodItem(&myBusiness, 24, "Deluxe Club Sandwich", 11.25, "Entree", 12);
    registerFoodItem(&myBusiness, 25, "Mediterranean Greek Salad", 10.25, "Starter", 8);
    registerFoodItem(&myBusiness, 26, "Fresh Caprese Salad", 9.25, "Starter", 7);
    registerFoodItem(&myBusiness, 27, "Homemade Tomato Soup", 6.25, "Starter", 6);
    registerFoodItem(&myBusiness, 28, "Hearty Chicken Noodle Soup", 7.25, "Starter", 8);
    registerFoodItem(&myBusiness, 29, "Vegetable Minestrone", 6.25, "Starter", 7);
    registerFoodItem(&myBusiness, 30, "Garlic Butter Shrimp", 13.25, "Starter", 15);
    registerFoodItem(&myBusiness, 31, "Crispy Calamari", 11.25, "Starter", 12);
    registerFoodItem(&myBusiness, 32, "Tomato Bruschetta", 8.25, "Starter", 8);
    registerFoodItem(&myBusiness, 33, "Loaded Nachos", 9.25, "Starter", 10);
    registerFoodItem(&myBusiness, 34, "Cheese Quesadilla", 10.25, "Starter", 12);
    registerFoodItem(&myBusiness, 35, "Fresh Guacamole", 7.25, "Starter", 5);
    registerFoodItem(&myBusiness, 36, "Salsa with Corn Chips", 5.25, "Starter", 3);
    registerFoodItem(&myBusiness, 37, "New York Cheesecake", 8.25, "Sweet", 5);
    registerFoodItem(&myBusiness, 38, "Warm Apple Pie", 7.25, "Sweet", 8);
    registerFoodItem(&myBusiness, 39, "Chocolate Brownie Sundae", 9.25, "Sweet", 6);
    registerFoodItem(&myBusiness, 40, "Artisanal Ice Cream", 5.25, "Sweet", 2);
    registerFoodItem(&myBusiness, 41, "Italian Tiramisu", 9.25, "Sweet", 5);
    registerFoodItem(&myBusiness, 42, "French Creme Brulee", 8.25, "Sweet", 6);
    registerFoodItem(&myBusiness, 43, "Chocolate Mousse Cup", 7.25, "Sweet", 4);
    registerFoodItem(&myBusiness, 44, "Seasonal Fruit Salad", 6.25, "Sweet", 5);
    registerFoodItem(&myBusiness, 45, "Zesty Lemon Tart", 7.25, "Sweet", 7);
    registerFoodItem(&myBusiness, 46, "Vanilla Panna Cotta", 8.25, "Sweet", 5);
    registerFoodItem(&myBusiness, 47, "Cola Drink", 2.75, "Drink", 1);
    registerFoodItem(&myBusiness, 48, "Diet Cola", 2.75, "Drink", 1);
    registerFoodItem(&myBusiness, 49, "Lemon Lime Soda", 2.75, "Drink", 1);
    registerFoodItem(&myBusiness, 50, "Fresh Lemonade", 3.75, "Drink", 2);
    registerFoodItem(&myBusiness, 51, "Orange Juice", 4.25, "Drink", 2);
    registerFoodItem(&myBusiness, 52, "Apple Juice", 4.25, "Drink", 2);
    registerFoodItem(&myBusiness, 53, "Premium Coffee", 3.25, "Drink", 3);
    registerFoodItem(&myBusiness, 54, "Herbal Tea", 2.75, "Drink", 3);
    registerFoodItem(&myBusiness, 55, "Hot Chocolate", 4.25, "Drink", 4);
    registerFoodItem(&myBusiness, 56, "Fresh Milk", 3.25, "Drink", 1);
    registerFoodItem(&myBusiness, 57, "Bottled Water", 2.25, "Drink", 1);
    registerFoodItem(&myBusiness, 58, "Craft Beer", 6.25, "Drink", 2);
    registerFoodItem(&myBusiness, 59, "House Wine", 8.25, "Drink", 2);
    registerFoodItem(&myBusiness, 60, "Classic Margarita", 9.25, "Drink", 5);
    registerFoodItem(&myBusiness, 61, "Fresh Mojito", 8.75, "Drink", 5);
    registerFoodItem(&myBusiness, 62, "BBQ Chicken Pizza", 16.25, "Entree", 16);
    registerFoodItem(&myBusiness, 63, "Tropical Hawaiian Pizza", 15.25, "Entree", 15);
    registerFoodItem(&myBusiness, 64, "Garden Veggie Pizza", 14.25, "Entree", 14);
    registerFoodItem(&myBusiness, 65, "Supreme Deluxe Pizza", 17.25, "Entree", 18);
    registerFoodItem(&myBusiness, 66, "Buffalo Chicken Pizza", 16.25, "Entree", 16);
    registerFoodItem(&myBusiness, 67, "Meat Feast Pizza", 18.25, "Entree", 19);
    registerFoodItem(&myBusiness, 68, "White Garlic Pizza", 15.25, "Entree", 15);
    registerFoodItem(&myBusiness, 69, "Stuffed Calzone", 13.25, "Entree", 17);
    registerFoodItem(&myBusiness, 70, "Garlic Herb Knots", 6.25, "Starter", 8);
    registerFoodItem(&myBusiness, 71, "Soft Breadsticks", 5.25, "Starter", 6);
    registerFoodItem(&myBusiness, 72, "Mushroom Caps", 9.25, "Starter", 12);
    registerFoodItem(&myBusiness, 73, "Spinach Artichoke Blend", 10.25, "Starter", 10);
    registerFoodItem(&myBusiness, 74, "Loaded Potato Skins", 8.25, "Starter", 12);
    registerFoodItem(&myBusiness, 75, "Chicken Quesadilla", 11.25, "Entree", 14);
    registerFoodItem(&myBusiness, 76, "Beef Burrito", 12.25, "Entree", 15);
    registerFoodItem(&myBusiness, 77, "Chicken Soft Tacos", 10.25, "Entree", 12);
    registerFoodItem(&myBusiness, 78, "Fish Tacos", 11.25, "Entree", 13);
    registerFoodItem(&myBusiness, 79, "Vegetable Tacos", 9.25, "Entree", 10);
    registerFoodItem(&myBusiness, 80, "Chicken Burrito Bowl", 13.25, "Entree", 14);
    registerFoodItem(&myBusiness, 81, "Sizzling Steak Fajitas", 17.25, "Entree", 18);
    registerFoodItem(&myBusiness, 82, "Chicken Fajitas", 15.25, "Entree", 16);
    registerFoodItem(&myBusiness, 83, "Garlic Shrimp Scampi", 18.25, "Entree", 20);
    registerFoodItem(&myBusiness, 84, "Lobster Roll", 20.25, "Entree", 15);
    registerFoodItem(&myBusiness, 85, "Maryland Crab Cakes", 16.25, "Entree", 16);
    registerFoodItem(&myBusiness, 86, "Fish Burger", 12.25, "Entree", 14);
    registerFoodItem(&myBusiness, 87, "Vegetable Wrap", 10.25, "Entree", 10);
    registerFoodItem(&myBusiness, 88, "Turkey Club", 12.25, "Entree", 12);
    registerFoodItem(&myBusiness, 89, "Reuben Sandwich", 13.25, "Entree", 14);
    registerFoodItem(&myBusiness, 90, "Philly Cheese Steak", 14.25, "Entree", 15);
    registerFoodItem(&myBusiness, 91, "Chicken Parmesan", 16.25, "Entree", 18);
    registerFoodItem(&myBusiness, 92, "Eggplant Parmesan", 14.25, "Entree", 16);
    registerFoodItem(&myBusiness, 93, "Italian Meatballs", 13.25, "Entree", 15);
    registerFoodItem(&myBusiness, 94, "Chicken Fried Rice", 12.25, "Entree", 14);
    registerFoodItem(&myBusiness, 95, "Beef Fried Rice", 13.25, "Entree", 15);
    registerFoodItem(&myBusiness, 96, "Vegetable Fried Rice", 11.25, "Entree", 12);
    registerFoodItem(&myBusiness, 97, "Chicken Lo Mein", 13.25, "Entree", 15);
    registerFoodItem(&myBusiness, 98, "Beef Lo Mein", 14.25, "Entree", 16);
    registerFoodItem(&myBusiness, 99, "Vegetable Lo Mein", 12.25, "Entree", 13);
    registerFoodItem(&myBusiness, 100, "Kung Pao Chicken", 15.25, "Entree", 17);
    registerFoodItem(&myBusiness, 101, "Sweet & Sour Pork", 14.25, "Entree", 16);
    registerFoodItem(&myBusiness, 102, "Mongolian Beef", 16.25, "Entree", 18);
    registerFoodItem(&myBusiness, 103, "General Tso Chicken", 15.25, "Entree", 17);
    registerFoodItem(&myBusiness, 104, "Orange Chicken", 15.25, "Entree", 17);
    registerFoodItem(&myBusiness, 105, "Beef with Broccoli", 14.25, "Entree", 15);
    registerFoodItem(&myBusiness, 106, "Chicken with Broccoli", 13.25, "Entree", 14);

    int user_choice, transaction_code, table_number, product_code, product_amount;
    char service_method[15];
    
    printf("=== Food Service Management Platform ===\n");
    
    while(1) {
        printf("\nAvailable Operations:\n");
        printf("1. View Food Catalog\n");
        printf("2. Initiate New Transaction\n");
        printf("3. Append Items to Order\n");
        printf("4. Alter Order Status\n");
        printf("5. Void Transaction\n");
        printf("6. Compute Transaction Total\n");
        printf("7. Print Customer Receipt\n");
        printf("8. Process Cooking Pipeline\n");
        printf("9. Generate Business Insights\n");
        printf("10. Terminate Application\n");
        printf("Select operation: ");
        scanf("%d", &user_choice);
        
        if(user_choice == 1) {
            showFoodMenu(&myBusiness);
        }
        else if(user_choice == 2) {
            printf("Enter table identifier: ");
            scanf("%d", &table_number);
            printf("Specify service method (dine-in/takeout): ");
            scanf("%s", service_method);
            transaction_code = createNewOrder(&myBusiness, table_number, service_method);
            if(transaction_code != -1) {
                printf("Transaction established! Reference: %d\n", transaction_code);
            }
        }
        else if(user_choice == 3) {
            printf("Enter transaction reference: ");
            scanf("%d", &transaction_code);
            printf("Enter product code: ");
            scanf("%d", &product_code);
            printf("Enter product quantity: ");
            scanf("%d", &product_amount);
            includeProductInOrder(&myBusiness, transaction_code, product_code, product_amount);
        }
        else if(user_choice == 4) {
            printf("Enter transaction reference: ");
            scanf("%d", &transaction_code);
            printf("Update status (waiting/cooking/complete/delivered/voided): ");
            scanf("%s", service_method);
            modifyOrderState(&myBusiness, transaction_code, service_method);
        }
        else if(user_choice == 5) {
            printf("Enter transaction to void: ");
            scanf("%d", &transaction_code);
            removeOrder(&myBusiness, transaction_code);
        }
        else if(user_choice == 6) {
            printf("Enter transaction reference: ");
            scanf("%d", &transaction_code);
            computeOrderTotal(&myBusiness, transaction_code);
        }
        else if(user_choice == 7) {
            printf("Enter transaction reference: ");
            scanf("%d", &transaction_code);
            generateReceipt(&myBusiness, transaction_code);
        }
        else if(user_choice == 8) {
            handleCookingLine(&myBusiness);
        }
        else if(user_choice == 9) {
            produceAnalytics(&myBusiness);
        }
        else if(user_choice == 10) {
            printf("Application terminated successfully!\n");
            exit(0);
        }
        else {
            printf("Invalid selection! Please try again.\n");
        }
    }
    
    return 0;
}

void setupBusiness(FoodBusiness* biz) {
    biz->total_menu_items = 0;
    biz->active_orders = 0;
    biz->next_transaction_id = 5001;
    biz->line_start = 0;
    biz->line_end = -1;
    biz->total_earnings = 0.0;
    
    for(int i = 0; i < 24; i++) {
        biz->hourly_orders[i] = 0;
    }
}

void registerFoodItem(FoodBusiness* biz, int code, char* name, double price, char* type, int cook_duration) {
    if(biz->total_menu_items >= MAX_FOOD_ITEMS) {
        printf("Food catalog capacity reached!\n");
        return;
    }
    
    FoodItem* new_item = &biz->food_menu[biz->total_menu_items];
    new_item->code = code;
    strcpy(new_item->item_name, name);
    new_item->cost = price;
    strcpy(new_item->type, type);
    new_item->cook_time = cook_duration;
    
    biz->total_menu_items++;
}

void showFoodMenu(FoodBusiness* biz) {
    printf("\n=== FOOD SERVICE CATALOG ===\n");
    printf("%-4s %-25s %-15s %-10s %s\n", "Code", "Product Name", "Classification", "Price", "Prep Time");
    printf("----------------------------------------------------------------------------\n");
    
    for(int i = 0; i < biz->total_menu_items; i++) {
        FoodItem* current = &biz->food_menu[i];
        printf("%-4d %-25s %-15s $%-9.2f %d minutes\n", 
               current->code, current->item_name, current->type, current->cost, current->cook_time);
    }
}

int createNewOrder(FoodBusiness* biz, int table_num, char* service_mode) {
    if(biz->active_orders >= MAX_CUSTOMER_ORDERS) {
        printf("Maximum order capacity reached!\n");
        return -1;
    }
    
    CustomerTransaction* new_transaction = &biz->all_orders[biz->active_orders];
    new_transaction->transaction_id = biz->next_transaction_id++;
    new_transaction->customer_table = table_num;
    strcpy(new_transaction->service_mode, service_mode);
    new_transaction->timestamp = time(NULL);
    new_transaction->product_count = 0;
    strcpy(new_transaction->current_state, "waiting");
    new_transaction->subtotal_cost = 0.0;
    
    // Track order timing for analytics
    struct tm* time_data = localtime(&new_transaction->timestamp);
    biz->hourly_orders[time_data->tm_hour]++;
    
    biz->active_orders++;
    
    // Collect order items
    int continue_adding = 1;
    while(continue_adding && new_transaction->product_count < MAX_ITEMS_PER_ORDER) {
        int item_code, item_quantity;
        printf("Enter product code (0 to finish): ");
        scanf("%d", &item_code);
        
        if(item_code == 0) break;
        
        printf("Enter quantity: ");
        scanf("%d", &item_quantity);
        
        includeProductInOrder(biz, new_transaction->transaction_id, item_code, item_quantity);
        
        printf("Add another product? (1=Yes, 0=No): ");
        scanf("%d", &continue_adding);
    }
    
    // Queue for preparation
    addToCookingLine(biz, new_transaction);
    
    return new_transaction->transaction_id;
}

void includeProductInOrder(FoodBusiness* biz, int order_code, int product_code, int amount) {
    int order_index = findTransactionByCode(biz, order_code);
    if(order_index == -1) {
        printf("Transaction not located!\n");
        return;
    }
    
    CustomerTransaction* transaction = &biz->all_orders[order_index];
    
    if(transaction->product_count >= MAX_ITEMS_PER_ORDER) {
        printf("Order item limit reached!\n");
        return;
    }
    
    int product_index = findFoodItemByCode(biz, product_code);
    if(product_index == -1) {
        printf("Product not found in catalog!\n");
        return;
    }
    
    FoodItem* food_product = &biz->food_menu[product_index];
    
    OrderedProduct* order_product = &transaction->products[transaction->product_count];
    order_product->food_code = product_code;
    order_product->amount = amount;
    order_product->item_total = food_product->cost * amount;
    
    transaction->subtotal_cost += order_product->item_total;
    transaction->product_count++;
    
    printf("Added %d units of %s to order %d\n", amount, food_product->item_name, order_code);
}

void modifyOrderState(FoodBusiness* biz, int order_code, char* new_state) {
    int order_index = findTransactionByCode(biz, order_code);
    if(order_index == -1) {
        printf("Transaction not located!\n");
        return;
    }
    
    CustomerTransaction* transaction = &biz->all_orders[order_index];
    strcpy(transaction->current_state, new_state);
    
    printf("Order %d status updated to: %s\n", order_code, new_state);
    
    // Finalize transaction when delivered
    if(strcmp(new_state, "delivered") == 0) {
        computeOrderTotal(biz, order_code);
        biz->total_earnings += transaction->grand_total;
    }
}

void removeOrder(FoodBusiness* biz, int order_code) {
    int order_index = findTransactionByCode(biz, order_code);
    if(order_index == -1) {
        printf("Transaction not located!\n");
        return;
    }
    
    CustomerTransaction* transaction = &biz->all_orders[order_index];
    
    if(strcmp(transaction->current_state, "delivered") == 0) {
        printf("Cannot void delivered orders!\n");
        return;
    }
    
    strcpy(transaction->current_state, "voided");
    printf("Order %d has been voided.\n", order_code);
}

void computeOrderTotal(FoodBusiness* biz, int order_code) {
    int order_index = findTransactionByCode(biz, order_code);
    if(order_index == -1) {
        printf("Transaction not located!\n");
        return;
    }
    
    CustomerTransaction* transaction = &biz->all_orders[order_index];
    
    transaction->tax_amount = transaction->subtotal_cost * SALES_TAX;
    transaction->service_fee = transaction->subtotal_cost * SERVICE_FEE;
    transaction->grand_total = transaction->subtotal_cost + transaction->tax_amount + transaction->service_fee;
    
    printf("Total computed for order %d. Amount: $%.2f\n", order_code, transaction->grand_total);
}

void generateReceipt(FoodBusiness* biz, int order_code) {
    int order_index = findTransactionByCode(biz, order_code);
    if(order_index == -1) {
        printf("Transaction not located!\n");
        return;
    }
    
    CustomerTransaction* transaction = &biz->all_orders[order_index];
    
    printf("\n=== CUSTOMER RECEIPT ===\n");
    printf("Transaction ID: %d\n", transaction->transaction_id);
    printf("Table: %d\n", transaction->customer_table);
    printf("Service Type: %s\n", transaction->service_mode);
    
    char time_buffer[30];
    getFormattedTime(time_buffer);
    printf("Order Time: %s", ctime(&transaction->timestamp));
    printf("Current Status: %s\n", transaction->current_state);
    
    printf("\n%-25s %-8s %-10s %s\n", "Product", "Quantity", "Unit Price", "Line Total");
    printf("------------------------------------------------------------\n");
    
    for(int i = 0; i < transaction->product_count; i++) {
        OrderedProduct* item = &transaction->products[i];
        int product_index = findFoodItemByCode(biz, item->food_code);
        if(product_index != -1) {
            FoodItem* food_item = &biz->food_menu[product_index];
            printf("%-25s %-8d $%-9.2f $%-8.2f\n", 
                   food_item->item_name, item->amount, food_item->cost, item->item_total);
        }
    }
    
    printf("\nSubtotal: $%.2f\n", transaction->subtotal_cost);
    printf("Sales Tax (8%%): $%.2f\n", transaction->tax_amount);
    printf("Service Charge (10%%): $%.2f\n", transaction->service_fee);
    printf("FINAL AMOUNT: $%.2f\n", transaction->grand_total);
    printf("============================\n");
}

void handleCookingLine(FoodBusiness* biz) {
    printf("\n=== KITCHEN PRODUCTION LINE ===\n");
    
    if(isCookingLineEmpty(biz)) {
        printf("No orders in production queue.\n");
        return;
    }
    
    // Process cooking pipeline
    while(!isCookingLineEmpty(biz)) {
        CustomerTransaction* transaction = removeFromCookingLine(biz);
        if(transaction && strcmp(transaction->current_state, "voided") != 0) {
            printf("Processing Order %d (Table %d) - Status: %s\n", 
                   transaction->transaction_id, transaction->customer_table, transaction->current_state);
            
            // Advance order through production stages
            if(strcmp(transaction->current_state, "waiting") == 0) {
                modifyOrderState(biz, transaction->transaction_id, "cooking");
                printf("Order %d now in preparation.\n", transaction->transaction_id);
            } else if(strcmp(transaction->current_state, "cooking") == 0) {
                modifyOrderState(biz, transaction->transaction_id, "complete");
                printf("Order %d preparation completed.\n", transaction->transaction_id);
            }
        }
    }
}

void produceAnalytics(FoodBusiness* biz) {
    printf("\n=== BUSINESS PERFORMANCE METRICS ===\n");
    
    // Revenue tracking
    printf("Total Revenue: $%.2f\n", biz->total_earnings);
    
    // Peak period analysis
    printf("\nPeak Business Hours:\n");
    int max_volume = 0;
    for(int i = 0; i < 24; i++) {
        if(biz->hourly_orders[i] > max_volume) {
            max_volume = biz->hourly_orders[i];
        }
    }
    
    for(int i = 0; i < 24; i++) {
        if(biz->hourly_orders[i] == max_volume && max_volume > 0) {
            printf("Peak: %02d:00 with %d transactions\n", i, max_volume);
        }
    }
    
    // Product popularity analysis
    printf("\nTop Performing Products:\n");
    int product_popularity[MAX_FOOD_ITEMS] = {0};
    
    for(int i = 0; i < biz->active_orders; i++) {
        CustomerTransaction* transaction = &biz->all_orders[i];
        for(int j = 0; j < transaction->product_count; j++) {
            OrderedProduct* item = &transaction->products[j];
            product_popularity[item->food_code - 1] += item->amount;
        }
    }
    
    // Sort products by popularity
    for(int i = 0; i < biz->total_menu_items - 1; i++) {
        for(int j = 0; j < biz->total_menu_items - i - 1; j++) {
            if(product_popularity[j] < product_popularity[j + 1]) {
                // Exchange popularity data
                int temp_pop = product_popularity[j];
                product_popularity[j] = product_popularity[j + 1];
                product_popularity[j + 1] = temp_pop;
                
                // Exchange product data
                FoodItem temp_item = biz->food_menu[j];
                biz->food_menu[j] = biz->food_menu[j + 1];
                biz->food_menu[j + 1] = temp_item;
            }
        }
    }
    
    printf("Most Popular Items:\n");
    for(int i = 0; i < 3 && i < biz->total_menu_items; i++) {
        if(product_popularity[i] > 0) {
            printf("%d. %s - Ordered %d times\n", 
                   i + 1, biz->food_menu[i].item_name, product_popularity[i]);
        }
    }
}

// Cooking line management
void addToCookingLine(FoodBusiness* biz, CustomerTransaction* transaction) {
    if(isCookingLineFull(biz)) {
        printf("Cooking queue at capacity!\n");
        return;
    }
    
    biz->line_end = (biz->line_end + 1) % MAX_CUSTOMER_ORDERS;
    biz->cooking_line[biz->line_end] = transaction;
}

CustomerTransaction* removeFromCookingLine(FoodBusiness* biz) {
    if(isCookingLineEmpty(biz)) {
        return NULL;
    }
    
    CustomerTransaction* transaction = biz->cooking_line[biz->line_start];
    biz->line_start = (biz->line_start + 1) % MAX_CUSTOMER_ORDERS;
    return transaction;
}

int isCookingLineEmpty(FoodBusiness* biz) {
    return (biz->line_end + 1) % MAX_CUSTOMER_ORDERS == biz->line_start;
}

int isCookingLineFull(FoodBusiness* biz) {
    return (biz->line_end + 2) % MAX_CUSTOMER_ORDERS == biz->line_start;
}

// Helper implementations
int findFoodItemByCode(FoodBusiness* biz, int code) {
    for(int i = 0; i < biz->total_menu_items; i++) {
        if(biz->food_menu[i].code == code) {
            return i;
        }
    }
    return -1;
}

int findTransactionByCode(FoodBusiness* biz, int transaction_code) {
    for(int i = 0; i < biz->active_orders; i++) {
        if(biz->all_orders[i].transaction_id == transaction_code) {
            return i;
        }
    }
    return -1;
}

void getFormattedTime(char* output) {
    time_t current = time(NULL);
    struct tm* time_details = localtime(&current);
    strftime(output, 30, "%Y-%m-%d %H:%M:%S", time_details);
}

// Additional utility functions
void sortOrdersByTimestamp(CustomerTransaction arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j].timestamp > arr[j+1].timestamp) {
                CustomerTransaction temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int locateOrderByCode(FoodBusiness* biz, int order_code) {
    int low = 0;
    int high = biz->active_orders - 1;
    
    while(low <= high) {
        int mid = low + (high - low) / 2;
        
        if(biz->all_orders[mid].transaction_id == order_code) {
            return mid;
        }
        
        if(biz->all_orders[mid].transaction_id < order_code) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return -1;
}
