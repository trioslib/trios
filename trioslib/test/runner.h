/*
Copyright (c) 2010 IBOPE Media, Igor dos Santos Montagner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "minunit.h"
#include <signal.h>

extern test test0;

volatile int test_counter, num_tests = __COUNTER__, tests_passed = 0;

void run_test(int t) {
    test test_func = *(&test0 + t);
    #ifdef BEFORE_TEST
        BEFORE_TEST();
    #endif
    mux_run_test(test_func);
    #ifdef AFTER_TEST
        AFTER_TEST();
    #endif
}

void test_runner()
{
    test test_func;
    for (test_counter = 0; test_counter < num_tests; test_counter++) {
        test_func = *(&test0 + test_counter);
        #ifdef BEFORE_TEST
            BEFORE_TEST();
        #endif
        tests_passed += mux_run_test(test_func);
        #ifdef AFTER_TEST
            AFTER_TEST();
        #endif
    }
    printf("%d of %d tests passed\n", tests_passed, num_tests);
}

void segfault_handler(int signum)
{
    printf("\e[1;1m\e[1;41m\e[1;37mSEGMENTATION FAULT\e[0m " 
               "in %s:%s \n", __BASE_FILE__, test_name);
    exit(-1);
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, segfault_handler);
    printf("Testing: %s\n", __BASE_FILE__);
    if (argc < 2) {
        test_runner();
    } else {
        int t = atoi(argv[1]);
        run_test(t);
    }
    return 0;
}

