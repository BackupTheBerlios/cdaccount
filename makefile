SRC_FILES = main.c read_cd.c file_handle.c permis.c mount.c
OBJ_FILES = $(patsubst %.c, %.o, ${SRC_FILES})
DEP_FILES = $(patsubst %.c, %.d, ${SRC_FILES})
vpath %.c src
vpath %.h src
VPATH = src
CFLAGS = -c -g
LDFLAGS = -g

cdaccount: ${OBJ_FILES}
	gcc ${LDFLAGS}  ${OBJ_FILES} -o cdaccount
%.o:%.c
	gcc ${CFLAGS} $< -o $@
clean:
	rm *.o appexp

include ${DEP_FILES}
%.d: %.c
	gcc -M $(CFLAGS) $< > $@
	gcc -M $< | sed s/\\.o/.d/ > $@
