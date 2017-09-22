src = punch-tape.c
obj = $(src:.c=.o)

CFLAGS += `pkg-config --cflags --libs cairo`
LDFLAGS += `cups-config --cflags --libs`

punch-tape: $(obj)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -l png

.PHONY: clean
clean:
	rm -f $(obj) punch-tape