CC = clang-with-asan
CFLAGS = -Wall -Wextra -Werror -MMD -fno-sanitize=integer
LDFLAGS = -lm

ifdef NREADLINE
	CFLAGS += -DNREADLINE
else
	LDFLAGS += -lreadline
endif

GENERATED_HEADERS = grammar.l.h grammar.y.h
OBJS = arena.o ast.o eval.o eval_dict.o eval_list.o eval_refs.o eval_types.o \
	exception.o grammar.l.o grammar.y.o mm.o parser.o refs.o repl.o

TESTS_1 = simple_math simple_print algo_fizzbuzz algo_csum algo_join \
	algo_bubble algo_bubble_str stress_int stress_str multiple_refs \
	long_chain transpose ordered_fractions # champernowne bouncy_numbers
TESTS_2 = $(TESTS_1) dict_ops long_chain_dict tree dict_resize stress_struct
TESTS_3 = $(TESTS_2) self_cycle simple_recursive simple_rep long_loops \
	linked_list dense_graph compacting

test: test3
test1: $(TESTS_1:=-result)
test2: $(TESTS_2:=-result)
test3: $(TESTS_3:=-result)

subpython: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

tests/%-expected.txt: tests/%.py
	grep '# output' $< | sed 's/# output //' > $@

tests/%-actual.txt: tests/%.py subpython
	./subpython `grep '# -' $< | sed 's/#//'` $< > $@

%-result: tests/%-expected.txt tests/%-actual.txt
	diff -u $^ && echo PASSED test $(@F:-result=). || (echo FAILED test $(@F:-result=). Aborting.; false)

clean:
	rm -f *.d *.o subpython tests/*.txt

.PRECIOUS: tests/%-expected.txt tests/%-actual.txt
