#pragma once
#include <stdio.h>

typedef struct {
    char *text;
    int result;
} test;

#define DEF_TEST(S) \
   test _CURRENT_TEST = { S }; \
   _CURRENT_TEST.result = 0;

#define RUN_TEST(T) \
    test _test_##T = T(); \
    char* _status_text_for_##T = "\033[32mPASS "; \
    \
    if (_test_##T.result != 0) { \
        _status_text_for_##T = "\033[31mFAIL!"; \
    } \
    \
    printf("%s\033[m (%s) %s\n", _status_text_for_##T, #T, _test_##T.text);

#define DO_TEST(A,S) \
    if (!(A)) { \
        _CURRENT_TEST.result = 1; \
        printf("\033[31mFAILED: %s\033[m\n", S); \
    }

#define END_TEST \
    return _CURRENT_TEST
