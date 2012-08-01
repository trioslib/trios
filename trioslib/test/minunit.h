/*

Based on: http://www.jera.com/techinfo/jtns/jtn002.html

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

#ifndef __MINUNIT2__
#define __MINUNIT2__

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do {  char *message = test(); tests_run++; \
                            if (message) return message; } while (0)

extern int tests_run;

/* Modifications start here */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *(*test)();
char test_name[256];

#define VAR_NAME2(name, i) test test##i = name;

#define VAR_NAME(name, i) VAR_NAME2(name, i)

#define UTEST(name) \
char *name(); VAR_NAME(name, __COUNTER__) char *name() { strcpy(test_name, #name);


#define TEST_END return NULL; } 


int mux_run_test(test t)
{
    char *res = t();
    if (!res) {
        printf("\e[1;32m\e[1;42mOK\e[0m " 
               "in %s:%s \n", __FILE__, test_name);
        return 1;
    } else {
        printf("\e[1;31m\e[1;41mNO\e[0m " 
               "in %s:%s => %s\n", __FILE__, test_name, res);
        return 0;     
    }
}

#endif



