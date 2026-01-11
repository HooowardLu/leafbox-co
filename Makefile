X64_C_FILES = 	\
			arch/x64/co.S \
			coro/co.c \
			sched/sched.c \

ARM64_C_FILES = 	\
			arch/arm64/co.S \
			coro/co.c \
			sched/sched.c \

INCLUDES = -I.

test1:
	gcc $(INCLUDES) $(X64_C_FILES) test1.c
	ls -hl a.out

test2:
	gcc $(INCLUDES) $(X64_C_FILES) test2.c
	ls -hl a.out

arm64_test1:
	aarch64-none-linux-gnu-gcc -static $(INCLUDES) $(ARM64_C_FILES) test1.c
	ls -hl a.out

clean:
	find . -name "*.o"|xargs rm -rf
	find . -name "a.out"|xargs rm -rf