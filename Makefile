CC = gcc
CCOPTS = -c -std=gnu99 -O2 -Wall -fdata-sections -ffunction-sections
LDOPTS = -Wl,--gc-sections -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
DEPS = $(OBJS:%.o=%.d)
BIN = vegatool

$(BIN) : $(OBJS)
	$(CC) -o $@ $^ $(LDOPTS)

-include $(DEPS)

%.o: %.c
	$(CC) $(CCOPTS) -MMD -c $< -o $@

.phony : clean
clean:
	rm -f $(BIN) *.o *.d

.phony : install
install: $(BIN) 
	sudo cp $(BIN) /usr/local/bin

