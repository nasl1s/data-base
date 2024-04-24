CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: build_db

rebuild: clean all

clean:
	rm -rf *.o *.a ../build/build_db

format:
	clang-format -i *.c *.h

check:
	clang-format -n *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h




build_db: 
	$(CC) $(CFLAGS) modules_db.c master_levels.c master_modules.c master_status_events.c -o ../build/build_db



