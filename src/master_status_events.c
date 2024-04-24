#include <stdio.h>
#include <string.h>
#include <time.h>

#include "database.h"

#define DB "../materials/master_status_events.db"

DEF_SELECT(STATUS_EVENTS);
DEF_INSERT(STATUS_EVENTS);
DEF_UPDATE(STATUS_EVENTS);
DEF_DELETE(STATUS_EVENTS);

void output_select_status_events(int *error) {
    int count = -1;
    printf("Specify the number of records or enter \"-1\" to output all of them: ");
    if (!scanf("%d", &count))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb");
        fseek(db, 0, SEEK_END);
        if (count == -1) count = ftell(db) / sizeof(STATUS_EVENTS);
        rewind(db);

        for (int i = 0; i < count; i++) {
            STATUS_EVENTS record = select_STATUS_EVENTS(db, i);
            printf("%d %d %d %s %s\n", record.event_id, record.module_id, record.module_new_status,
                   record.modification_date, record.modification_time);
        }
        fclose(db);
    }
}

void insert_status_events(int *error) {
    STATUS_EVENTS record;
    FILE *db = fopen(DB, "ab");
    record.event_id = ftell(db) / sizeof(STATUS_EVENTS);
    printf(
        "> Input for new record \"module_id module_new_status modification_date(DD.MM.YYYY) "
        "modification_time(HH:MM:SS)\": ");
    if (scanf("%d %d %11s %9s", &record.module_id, &record.module_new_status, record.modification_date,
              record.modification_time) < 4)
        *error = 1;
    else {
        insert_STATUS_EVENTS(db, &record);
        printf("SUCCESS. Inserted at position: %d\n", record.event_id);
    }
    fclose(db);
}

void update_status_events(int *error) {
    STATUS_EVENTS record;
    FILE *db = fopen(DB, "rb+");
    printf("> Update for existing record \"id name level cell to_delete\": ");
    if (scanf("%d %d %d %11s %9s", &record.event_id, &record.module_id, &record.module_new_status,
              record.modification_date, record.modification_time) < 5)
        *error = 1;
    else {
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(STATUS_EVENTS);
        int i = 0, found = 0;
        for (; i < size; i++) {
            STATUS_EVENTS tmp = select_STATUS_EVENTS(db, i);
            if (record.event_id == tmp.event_id) {
                found = 1;
                break;
            }
        }
        if (found) {
            update_STATUS_EVENTS(db, i, &record);
            printf("SUCCESS. Updated at id: %d\n", record.event_id);
        } else
            printf("ERROR. Record not found\n");
    }
    fclose(db);
}

void delete_status_events(int *error) {
    int id;
    FILE *db = fopen(DB, "rb+");
    printf("> Input id: ");
    if (!scanf("%d", &id))
        *error = 1;
    else {
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(STATUS_EVENTS);
        int i = 0, found = 0;
        for (; i < size + 1; i++) {
            STATUS_EVENTS tmp = select_STATUS_EVENTS(db, i);
            if (id == tmp.event_id) {
                found = 1;
                break;
            }
        }
        if (found) {
            delete_STATUS_EVENTS(db, DB, i);
            printf("SUCCESS. Record with id %d deleted\n", id);
        } else
            printf("ERROR. Record not found\n");
    }
    fclose(db);
}

void show_all_active_modules() {
    FILE *db = fopen(DB, "rb+");
    fseek(db, 0, SEEK_END);
    int size = ftell(db) / sizeof(STATUS_EVENTS);
    rewind(db);
    for (int i = 0; i < size; i++) {
        STATUS_EVENTS record = select_STATUS_EVENTS(db, i);
        if (record.module_new_status == 1) {
            printf("%d %d %d %s %s\n", record.event_id, record.module_id, record.module_new_status,
                   record.modification_date, record.modification_time);
        }
    }
    fclose(db);
}

void disable_all_modules() {
    FILE *db = fopen(DB, "rb+");
    fseek(db, 0, SEEK_END);
    int size = ftell(db) / sizeof(STATUS_EVENTS);
    rewind(db);
    for (int i = 0; i < size; i++) {
        STATUS_EVENTS record = select_STATUS_EVENTS(db, i);
        if (record.module_new_status == 1) {
            record.module_new_status = 0;
            update_STATUS_EVENTS(db, i, &record);
        }
    }
    fclose(db);
}

void set_module_protected_by_id(int *error) {
    printf("> Enter id: ");
    int id;
    if (!scanf("%d", &id))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb+");
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(STATUS_EVENTS);

        STATUS_EVENTS record;
        record.event_id = size;
        size++;
        record.module_id = id;
        record.module_new_status = 0;
        char time_str[10];
        char date_str[12];
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);
        strftime(date_str, sizeof(date_str), "%d.%m.%Y", time_info);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
        strcpy(record.modification_date, date_str);
        strcpy(record.modification_time, time_str);
        insert_STATUS_EVENTS(db, &record);

        record.event_id = size;
        size++;
        record.module_new_status = 1;
        current_time = time(NULL);
        time_info = localtime(&current_time);
        strftime(date_str, sizeof(date_str), "%d.%m.%Y", time_info);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
        strcpy(record.modification_date, date_str);
        strcpy(record.modification_time, time_str);
        insert_STATUS_EVENTS(db, &record);

        record.event_id = size;
        record.module_new_status = -20;
        current_time = time(NULL);
        time_info = localtime(&current_time);
        strftime(date_str, sizeof(date_str), "%d.%m.%Y", time_info);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
        strcpy(record.modification_date, date_str);
        strcpy(record.modification_time, time_str);
        insert_STATUS_EVENTS(db, &record);

        printf("SUCCESS\n");
        fclose(db);
    }
}

void bs_id_status_events(int *error) {
    printf("> Enter id: ");
    int id;
    if (!scanf("%d", &id))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb+");
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(STATUS_EVENTS);

        STATUS_EVENTS record;
        record.event_id = record.module_id = record.module_new_status = -1;
        strcpy(record.modification_date, "00.00.0000");
        strcpy(record.modification_time, "00:00:00");

        int down = 0, up = size - 1;
        while (down <= up) {
            int mid = (down + up) / 2;
            int offset = mid * sizeof(STATUS_EVENTS);
            fseek(db, offset, SEEK_SET);
            fread(&record, sizeof(STATUS_EVENTS), 1, db);
            if (record.event_id > id)
                up = mid - 1;
            else if (record.event_id < id)
                down = mid + 1;
            else
                break;
        }

        if (record.event_id == -1)
            printf("Record not found\n");
        else
            printf("%d %d %d %s %s\n", record.event_id, record.module_id, record.module_new_status,
                   record.modification_date, record.modification_time);
        fclose(db);
    }
}