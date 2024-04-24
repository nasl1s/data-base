#ifndef DATABASE_H
#define DATABASE_H

#ifndef ENTITY
#define ENTITY
#endif

#define FILEPATH_MAX_LEN 1025

#include <stdio.h>
#include <string.h>

typedef struct modules {
    int id;
    char name[30];
    int level;
    int cell;
    int to_delete;
} MODULES;

typedef struct levels {
    int level;
    int cells_count;
    int protected;
} LEVELS;

typedef struct status_events {
    int event_id;
    int module_id;
    int module_new_status;
    char modification_date[10 + 1];
    char modification_time[8 + 1];
} STATUS_EVENTS;

#define DEF_SELECT(ENTITY)                     \
    ENTITY select_##ENTITY(FILE *db, int id) { \
        int offset = id * sizeof(ENTITY);      \
        fseek(db, offset, SEEK_SET);           \
        ENTITY record;                         \
        fread(&record, sizeof(ENTITY), 1, db); \
        rewind(db);                            \
        return record;                         \
    }

#define DEF_INSERT(ENTITY)                          \
    int insert_##ENTITY(FILE *db, ENTITY *entity) { \
        fseek(db, 0, SEEK_END);                     \
        int position = ftell(db) / sizeof(ENTITY);  \
        fwrite(entity, sizeof(ENTITY), 1, db);      \
        fflush(db);                                 \
        rewind(db);                                 \
        return position;                            \
    }

#define DEF_UPDATE(ENTITY)                                  \
    int update_##ENTITY(FILE *db, int id, ENTITY *entity) { \
        int offset = id * sizeof(ENTITY);                   \
        fseek(db, offset, SEEK_SET);                        \
        fwrite(entity, sizeof(ENTITY), 1, db);              \
        fflush(db);                                         \
        rewind(db);                                         \
        return id;                                          \
    }

#define DEF_DELETE(ENTITY)                                   \
    int delete_##ENTITY(FILE *db, char *filepath, int pos) { \
        char buf[FILEPATH_MAX_LEN] = {'\0'};                 \
        strcpy(buf, filepath);                               \
        strcat(buf, "tmp");                                  \
        fseek(db, 0, SEEK_END);                              \
        int record_count = ftell(db) / sizeof(ENTITY);       \
        FILE *fpointer_tmp = fopen(buf, "wb");               \
        rewind(db);                                          \
        for (int i = 0; i < record_count; i++) {             \
            if (i != pos) {                                  \
                ENTITY record = select_##ENTITY(db, i);      \
                insert_##ENTITY(fpointer_tmp, &record);      \
            }                                                \
        }                                                    \
        rewind(fpointer_tmp);                                \
        fclose(db);                                          \
        fclose(fpointer_tmp);                                \
        remove(filepath);                                    \
        rename(buf, filepath);                               \
        return pos;                                          \
    }

void output_select_levels(int *error);
void output_select_modules(int *error);
void output_select_status_events(int *error);

void insert_levels(int *error);
void insert_modules(int *error);
void insert_status_events(int *error);

void update_levels(int *error);
void update_modules(int *error);
void update_status_events(int *error);

void delete_levels(int *error);
void delete_modules(int *error);
void delete_status_events(int *error);

void show_all_active_modules();
void disable_all_modules();
void mark_modules_to_delete(int *error);
void set_module_protected_by_id(int *error);
void move_module_by_id(int *error);
void protect_level(int *error);

void bs_id_modules(int *error);
void bs_id_levels(int *error);
void bs_id_status_events(int *error);

#endif