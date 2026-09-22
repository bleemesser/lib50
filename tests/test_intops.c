/*
 * test_intops.c --- Unit tests for intops
 *
 * Author: Beckett Lee-Messer
 * Created: 09-22-2026
 * Version: 1.0
 *
 * Description: Checks strtoint32 against valid input, malformed input and
 *              values outside the range of an int32_t.
 *
 */

#include "lib50/intops.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

static void check(bool cond, const char *what) {
    if (!cond) {
        printf("FAIL: %s\n", what);
        failures++;
    }
}

int main(void) {
    int32_t v = 0;

    check(strtoint32("0", &v) && v == 0, "zero");
    check(strtoint32("42", &v) && v == 42, "positive");
    check(strtoint32("-7", &v) && v == -7, "negative");
    check(strtoint32("  +3", &v) && v == 3, "leading space and sign");
    check(strtoint32("2147483647", &v) && v == INT32_MAX, "largest int32");
    check(strtoint32("-2147483648", &v) && v == INT32_MIN, "smallest int32");

    check(!strtoint32("", &v), "empty string");
    check(!strtoint32("abc", &v), "no digits");
    check(!strtoint32("12abc", &v), "trailing garbage");
    check(!strtoint32(NULL, &v), "null string");
    check(!strtoint32("1", NULL), "null output");
    check(!strtoint32("2147483648", &v) && errno == ERANGE, "overflow");
    check(!strtoint32("-2147483649", &v) && errno == ERANGE, "underflow");

    // a failed call must leave the output untouched
    v = 99;
    strtoint32("nope", &v);
    check(v == 99, "output preserved on failure");

    printf("%s\n", failures > 0 ? "some tests failed" : "all tests passed");
    return failures > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
