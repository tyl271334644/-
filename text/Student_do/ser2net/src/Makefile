
CFLAGS += -Wall
LDFLAGS=

APP	= ser2net
SRCS	= $(wildcard *.c)
OBJS	= $(patsubst %.c,%.o,$(SRCS))

all: $(APP)

$(APP): $(OBJS)
	$(CC) -static -o $(APP) $^ $(LDFLAGS)
	mv $(APP) ../

clean:
	rm -f $(APP) $(OBJS)


