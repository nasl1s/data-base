#include <stdio.h>

#include "database.h"

void print_menu();
int input_menu_option(int *error);
void print_tables();
int input_table(int *error);

void use_menu(int option, int table, int *error);
void select_option(int table, int *error);
void insert_option(int table, int *error);
void update_option(int table, int *error);
void delete_option(int table, int *error);
void binary_search_by_id(int table, int *error);

int main() {
    int menu_option = 0;

    do {
        int error = 0;
        int table = 0;

        print_menu();
        menu_option = input_menu_option(&error);
        if (!error && ((menu_option >= 1 && menu_option <= 4) || menu_option == 22)) {
            print_tables();
            table = input_table(&error);
        }
        if (!error && menu_option != 0) {
            use_menu(menu_option, table, &error);
        }

        if (error) printf("n/a\n");

        while (getchar() != '\n')
            ;

    } while (menu_option != 0);

    return 0;
}

void print_menu() {
    printf("Please choose one operation:\n");
    printf("\t1. SELECT\n");
    printf("\t2. INSERT\n");
    printf("\t3. UPDATE\n");
    printf("\t4. DELETE\n");
    printf("\t5. Show all active modules\n");
    printf("\t6. Disable all modules\n");
    printf("\t7. Mark modules by ids for delete\n");
    printf("\t8. Set protected mode for module by id\n");
    printf("\t9. Move module by id to specified memory level and cell\n");
    printf("\t10. Set protection flag of the specified memory level\n");
    printf("\t22. Find record by id using binary search\n");
    printf("\t0. Exit\n");
    printf("> ");
}

int input_menu_option(int *error) {
    int menu_option = 0;
    if (!scanf("%d", &menu_option)) *error = 1;
    return menu_option;
}

void print_tables() {
    printf("Please choose a table:\n");
    printf("\t1. Modules\n");
    printf("\t2. Levels\n");
    printf("\t3. Status events\n");
    printf("> ");
}

int input_table(int *error) {
    int table = 0;
    if (!scanf("%d", &table)) *error = 1;
    return table;
}

void use_menu(int option, int table, int *error) {
    if (option == 1) {
        select_option(table, error);
    } else if (option == 2) {
        insert_option(table, error);
    } else if (option == 3) {
        update_option(table, error);
    } else if (option == 4) {
        delete_option(table, error);
    } else if (option == 5) {
        show_all_active_modules();
    } else if (option == 6) {
        disable_all_modules();
        output_select_status_events(error);
    } else if (option == 7) {
        mark_modules_to_delete(error);
    } else if (option == 8) {
        set_module_protected_by_id(error);
    } else if (option == 9) {
        move_module_by_id(error);
    } else if (option == 10) {
        protect_level(error);
    } else if (option == 22) {
        binary_search_by_id(table, error);
    } else
        *error = 1;
}

void select_option(int table, int *error) {
    if (table == 1) {
        output_select_modules(error);
    } else if (table == 2) {
        output_select_levels(error);
    } else if (table == 3) {
        output_select_status_events(error);
    } else
        *error = 1;
}

void insert_option(int table, int *error) {
    if (table == 1) {
        insert_modules(error);
    } else if (table == 2) {
        insert_levels(error);
    } else if (table == 3) {
        insert_status_events(error);
    } else
        *error = 1;
}

void update_option(int table, int *error) {
    if (table == 1) {
        update_modules(error);
    } else if (table == 2) {
        update_levels(error);
    } else if (table == 3) {
        update_status_events(error);
    } else
        *error = 1;
}

void delete_option(int table, int *error) {
    if (table == 1) {
        delete_modules(error);
    } else if (table == 2) {
        delete_levels(error);
    } else if (table == 3) {
        delete_status_events(error);
    } else
        *error = 1;
}

void binary_search_by_id(int table, int *error) {
    if (table == 1) {
        bs_id_modules(error);
    } else if (table == 2) {
        bs_id_levels(error);
    } else if (table == 3) {
        bs_id_status_events(error);
    } else
        *error = 1;
}