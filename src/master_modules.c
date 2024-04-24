#include <stdio.h>
#include <string.h>

#include "database.h"

#define DB "../materials/master_modules.db"

DEF_SELECT(MODULES);
DEF_INSERT(MODULES);
DEF_UPDATE(MODULES);
DEF_DELETE(MODULES);

void output_select_modules(int *error) {
    int count = -1;
    printf("Specify the number of records or enter \"-1\" to output all of them: ");
    if (!scanf("%d", &count))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb");
        fseek(db, 0, SEEK_END);
        if (count == -1) count = ftell(db) / sizeof(MODULES);
        rewind(db);

        for (int i = 0; i < count; i++) {
            MODULES record = select_MODULES(db, i);
            printf("%d %s %d %d %d\n", record.id, record.name, record.level, record.cell, record.to_delete);
        }
    }
}

void insert_modules(int *error) {
    MODULES record;
    FILE *db = fopen(DB, "ab");
    record.id = ftell(db) / sizeof(MODULES);
    printf("> Input for new record \"name level cell to_delete\": ");
    if (scanf("%30s %d %d %d", record.name, &record.level, &record.cell, &record.to_delete) < 4)
        *error = 1;
    else {
        insert_MODULES(db, &record);
        printf("SUCCESS. Inserted at position: %d\n", record.id);
    }
    fclose(db);
}

void update_modules(int *error) {
    MODULES record;
    FILE *db = fopen(DB, "rb+");
    printf("> Update for existing record \"id name level cell to_delete\": ");
    if (scanf("%d %30s %d %d %d", &record.id, record.name, &record.level, &record.cell, &record.to_delete) <
        5)
        *error = 1;
    else {
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(MODULES);
        int i = 0, found = 0;
        for (; i < size; i++) {
            MODULES tmp = select_MODULES(db, i);
            if (record.id == tmp.id) {
                found = 1;
                break;
            }
        }
        if (found) {
            update_MODULES(db, i, &record);
            printf("SUCCESS. Updated at id: %d\n", record.id);
        } else
            printf("ERROR. Record not found\n");
    }
    fclose(db);
}

void delete_modules(int *error) {
    int id;
    FILE *db = fopen(DB, "rb+");
    printf("> Input id: ");
    if (!scanf("%d", &id))
        *error = 1;
    else {
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(MODULES);
        int i = 0, found = 0;
        for (; i < size; i++) {
            MODULES tmp = select_MODULES(db, i);
            if (id == tmp.id) {
                found = 1;
                break;
            }
        }
        if (found) {
            delete_MODULES(db, DB, i);
            printf("SUCCESS. Record with id %d deleted\n", id);
        } else
            printf("ERROR. Record not found\n");
    }
    fclose(db);
}

void mark_modules_to_delete(int *error) {
    printf("> Enter number of ids to delete: ");
    int number;
    if (!scanf("%d", &number))
        *error = 1;
    else {
        printf("> Enter %d ids to delete: ", number);
        FILE *db = fopen(DB, "rb+");
        for (int i = 0; i < number && !*error; i++) {
            int id;
            if (!scanf("%d", &id))
                *error = 1;
            else {
                MODULES record = select_MODULES(db, id);
                record.to_delete = 1;
                update_MODULES(db, id, &record);
            }
        }
        printf("SUCCESS\n");
        fclose(db);
    }
}

void move_module_by_id(int *error) {
    printf("> Enter id: ");
    int id;
    if (!scanf("%d", &id))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb+");
        fseek(db, 0, SEEK_END);

        MODULES record = select_MODULES(db, id);
        record.cell = 1;
        record.level = 1;
        update_MODULES(db, id, &record);

        printf("SUCCESS\n");
        fclose(db);
    }
}

void bs_id_modules(int *error) {
    printf("> Enter id: ");
    int id;
    if (!scanf("%d", &id))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb+");
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(MODULES);

        MODULES record;
        record.id = record.cell = record.level = record.to_delete = -1;
        strcpy(record.name, "dafuq");

        int down = 0, up = size - 1;
        while (down <= up) {
            int mid = (down + up) / 2;
            int offset = mid * sizeof(MODULES);
            fseek(db, offset, SEEK_SET);
            fread(&record, sizeof(MODULES), 1, db);
            if (record.id > id)
                up = mid - 1;
            else if (record.id < id)
                down = mid + 1;
            else
                break;
        }

        if (record.id == -1)
            printf("Record not found\n");
        else
            printf("%d %s %d %d %d\n", record.id, record.name, record.level, record.cell, record.to_delete);
        fclose(db);
    }
}