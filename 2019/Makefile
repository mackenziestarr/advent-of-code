CC:=clang++
CPPFLAGS:=-Wall -g -std=c++17
OUTDIR:=target

%: src/%.cpp
	@mkdir -p target
	@$(CC) $(CFLAGS) $(CPPFLAGS) $< -o ${OUTDIR}/$@
	@./${OUTDIR}/$@

%_test : src/%.cpp
	@mkdir -p target
	@$(CC) $(CFLAGS) -D RUN_TESTS $(CPPFLAGS) $< -o ${OUTDIR}/$@
	@./${OUTDIR}/$@

.PHONY: clean all

all: $(patsubst src/%.cpp, %, $(wildcard src/*.cpp))

clean:
	rm -rf *.out*
