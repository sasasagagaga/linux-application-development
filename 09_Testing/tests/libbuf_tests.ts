#include <check.h>
#include "libbuf.h"

float *a = 0;
long *ai = 0;

#suite Test libbuf
#test initialization_and_buf_free
    ck_assert(buf_capacity(a) == 0);
    ck_assert(buf_size(a) == 0);
    buf_push(a, 1.3f);
    ck_assert(buf_size(a) == 1);
    ck_assert(a[0] == (float)1.3f);
    buf_clear(a);
    ck_assert(buf_size(a) == 0);
    ck_assert(a != 0);
    buf_free(a);
    ck_assert(a == 0);

    buf_clear(a);
    ck_assert(buf_size(a) == 0);
    ck_assert(a == 0);

#test buf_push_and_get_operator
    for (int i = 0; i < 10000; i++)
        buf_push(ai, i);
    ck_assert(buf_size(ai) == 10000);
    int match = 0;
    for (int i = 0; i < (int)(buf_size(ai)); i++)
        match += ai[i] == i;
    ck_assert(match == 10000);
    buf_free(ai);

#test buf_grow_and_buf_trunc
    buf_grow(ai, 1000);
    ck_assert(buf_capacity(ai) == 1000);
    ck_assert(buf_size(ai) == 0);
    buf_trunc(ai, 100);
    ck_assert(buf_capacity(ai) == 100);
    buf_free(ai);

#test buf_pop_
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    buf_push(a, 1.4);
    ck_assert(buf_size(a) == 4);
    ck_assert(buf_pop(a) == (float)1.4f);
    buf_trunc(a, 3);
    ck_assert(buf_size(a) == 3);
    ck_assert(buf_pop(a) == (float)1.3f);
    ck_assert(buf_pop(a) == (float)1.2f);
    ck_assert(buf_pop(a) == (float)1.1f);
    ck_assert(buf_size(a) == 0);
    buf_free(a);

