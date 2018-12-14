CC=ocamlopt
SRCS = $(wildcard day_*.ml)
PROGS = $(patsubst %.ml,%.native,$(SRCS))

all: $(PROGS)

%.native: %.ml
	$(CC) -o $@ str.cmxa lib.ml $<
	./$@

clean:
	rm -rf *.cmi *.cmx *.cmo *.native _build *.o

.PHONY: clean default
