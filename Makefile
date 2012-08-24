# dwk – dynamic widget kit
# See LICENSE for copyright and license details

include config.mk

SRC = dwk.c draw.c div.c img.c
OBJ = ${SRC:.c=.o}

all: options dwk

options:
	@echo -e "dwk build options\nCFLAGS\t=${CFLAGS}\nLDFLAGS\t${LDFLAGS}\nCC\t${CC}"

${OBJ}: dwk.h draw.h div.h img.h

dwk: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f dwk ${OBJ}
