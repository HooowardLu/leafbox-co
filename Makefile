C_FILES = 	\
			arch/x64/co.S \
			coro/co.c \
			sched/sched.c \

INCLUDES = -I.

test1:
	gcc $(INCLUDES) $(C_FILES) test1.c
	ls -hl a.out

test2:
	gcc $(INCLUDES) $(C_FILES) test2.c
	ls -hl a.out

clean:
	find . -name "*.o"|xargs rm -rf
	find . -name "a.out"|xargs rm -rf