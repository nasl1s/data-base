#include <stdio.h>
#include <string.h>

#include "database.h"

#define DB "../materials/master_levels.db"

DEF_SELECT(LEVELS);
DEF_INSERT(LEVELS);
DEF_UPDATE(LEVELS);
DEF_DELETE(LEVELS);

void output_select_levels(int *error) {
    int count = -1;
    printf("Specify the number of records or enter \"-1\" to output all of them: ");
    if (!scanf("%d", &count))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb");
        fseek(db, 0, SEEK_END);
        if (count == -1) count = ftell(db) / sizeof(LEVELS);
        rewind(db);

        for (int i = 0; i < count; i++) {
            LEVELS record = select_LEVELS(db, i);
            printf("%d %d %d\n", record.level, record.cells_count, record.protected);
        }
    }
}

void insert_levels(int *error) {
    LEVELS record;
    FILE *db = fopen(DB, "ab");
    record.level = ftell(db) / sizeof(LEVELS) + 1;
    printf("> Input for new level %d \"cells_count protected\": ", record.level);
    if (scanf("%d %d", &record.cells_count, &record.protected) < 2)
        *error = 1;
    else {
        insert_LEVELS(db, &record);
        printf("SUCCESS. Inserted at position: %d\n", record.level);
    }
    fclose(db);
}

void update_levels(int *error) {
    LEVELS record;
    FILE *db = fopen(DB, "rb+");
    printf("> Input for existing level \"level_id cells_count protected\": ");
    if (scanf("%d %d %d", &record.level, &record.cells_count, &record.protected) < 3)
        *error = 1;
    else {
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(LEVELS);
        int i = 0, found = 0;
        for (; i < size; i++) {
            LEVELS tmp = select_LEVELS(db, i);
            if (record.level == tmp.level) {
                found = 1;
                break;
            }
        }
        if (found) {
            update_LEVELS(db, i, &record);
            printf("SUCCESS. Updated at id: %d\n", record.level);
        } else
            printf("ERROR. Record not found\n");
    }
    fclose(db);
}

void delete_levels(int *error) {
    int id;
    FILE *db = fopen(DB, "rb+");
    printf("> Input id: ");
    if (!scanf("%d", &id))
        *error = 1;
    else {
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(LEVELS);
        int i = 0, found = 0;
        for (; i < size + 1; i++) {
            LEVELS tmp = select_LEVELS(db, i);
            if (id == tmp.level) {
                found = 1;
                break;
            }
        }
        if (found) {
            delete_LEVELS(db, DB, i);
            printf("SUCCESS. Record with id %d deleted\n", id);
        } else
            printf("ERROR. Record not found\n");
    }
    fclose(db);
}

void protect_level(int *error) {
    printf("> Enter level: ");
    int level;
    if (!scanf("%d", &level))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb+");
        fseek(db, 0, SEEK_END);

        LEVELS record = select_LEVELS(db, level - 1);
        record.protected = 1;
        update_LEVELS(db, level - 1, &record);

        printf("SUCCESS\n");
        fclose(db);
    }
}

void bs_id_levels(int *error) {
    printf("> Enter id: ");
    int id;
    if (!scanf("%d", &id))
        *error = 1;
    else {
        FILE *db = fopen(DB, "rb+");
        fseek(db, 0, SEEK_END);
        int size = ftell(db) / sizeof(LEVELS);

        LEVELS record;
        record.level = record.cells_count = record.protected = -1;

        int down = 0, up = size - 1;
        while (down <= up) {
            int mid = (down + up) / 2;
            int offset = mid * sizeof(LEVELS);
            fseek(db, offset, SEEK_SET);
            fread(&record, sizeof(LEVELS), 1, db);
            if (record.level > id)
                up = mid - 1;
            else if (record.level < id)
                down = mid + 1;
            else
                break;
        }

        if (record.level == -1)
            printf("Record not found\n");
        else
            printf("%d %d %d\n", record.level, record.cells_count, record.protected);
        fclose(db);
    }
}