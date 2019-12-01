CC:=clang++
CPPFLAGS:=-Wall -g
OUTDIR:=target

%.o: %.cpp
	@mkdir -p target
	@$(CC) $(CFLAGS) $(CPPFLAGS) $< -o ${OUTDIR}/$@
	@./${OUTDIR}/$@

.PHONY: clean all

all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))

clean:
	rm -rf *.out*
