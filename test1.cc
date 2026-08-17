#include <iostream>
#include "coro/co.h"

Co *ma, *a;

void foo(void) {
    std::cout << "===> hello" << std::endl;
    co_ctx_swap(&a->ctx_, &ma->ctx_);
    std::cout << "===> resumed" << std::endl;
    co_ctx_swap(&a->ctx_, &ma->ctx_);
}


int main(int argc, char *argv[]) {
    ma = new Co("main", NULL);
    a = new Co("a", foo);

    co_ctx_swap(&ma->ctx_, &a->ctx_);
    printf("main coroutine here\n");
    co_ctx_swap(&ma->ctx_, &a->ctx_);
    printf("main coroutine finish\n");

    return 0;
}