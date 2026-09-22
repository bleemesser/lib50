/*
 * test_strconv.c --- Unit tests for strconv
 *
 * Author: Beckett Lee-Messer
 * Created: 09-22-2026
 * Version: 1.0
 *
 * Description: Checks each conversion against valid input, malformed input and
 *              values outside the range of the output type.
 *
 */

#include "lib50/strconv.h"

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

static void test_int32(void) {
    int32_t v = 0;

    check(strconv_to_int32("0", &v) && v == 0, "int32 zero");
    check(strconv_to_int32("42", &v) && v == 42, "int32 positive");
    check(strconv_to_int32("-7", &v) && v == -7, "int32 negative");
    check(strconv_to_int32("  +3", &v) && v == 3,
          "int32 leading space and sign");
    check(strconv_to_int32("2147483647", &v) && v == INT32_MAX,
          "int32 largest");
    check(strconv_to_int32("-2147483648", &v) && v == INT32_MIN,
          "int32 smallest");

    check(!strconv_to_int32("", &v), "int32 empty string");
    check(!strconv_to_int32("abc", &v), "int32 no digits");
    check(!strconv_to_int32("12abc", &v), "int32 trailing garbage");
    check(!strconv_to_int32(NULL, &v), "int32 null string");
    check(!strconv_to_int32("1", NULL), "int32 null output");
    check(!strconv_to_int32("2147483648", &v) && errno == ERANGE,
          "int32 overflow");
    check(!strconv_to_int32("-2147483649", &v) && errno == ERANGE,
          "int32 underflow");

    // a failed call must leave the output untouched
    v = 99;
    strconv_to_int32("nope", &v);
    check(v == 99, "int32 output preserved on failure");
}

static void test_uint32(void) {
    uint32_t v = 0;

    check(strconv_to_uint32("0", &v) && v == 0, "uint32 zero");
    check(strconv_to_uint32("  +42", &v) && v == 42,
          "uint32 leading space and sign");
    check(strconv_to_uint32("4294967295", &v) && v == UINT32_MAX,
          "uint32 largest");

    check(!strconv_to_uint32("", &v), "uint32 empty string");
    check(!strconv_to_uint32("12abc", &v), "uint32 trailing garbage");
    check(!strconv_to_uint32(NULL, &v), "uint32 null string");
    check(!strconv_to_uint32("1", NULL), "uint32 null output");
    check(!strconv_to_uint32("-1", &v) && errno == EINVAL, "uint32 negative");
    check(!strconv_to_uint32("  -0", &v) && errno == EINVAL,
          "uint32 negative zero");
    check(!strconv_to_uint32("4294967296", &v) && errno == ERANGE,
          "uint32 overflow");

    v = 99;
    strconv_to_uint32("nope", &v);
    check(v == 99, "uint32 output preserved on failure");
}

static void test_float(void) {
    float v = 0;

    check(strconv_to_float("0", &v) && v == 0.0f, "float zero");
    check(strconv_to_float("1.5", &v) && v == 1.5f, "float positive");
    check(strconv_to_float("  -2.25", &v) && v == -2.25f,
          "float leading space and sign");
    check(strconv_to_float("1e3", &v) && v == 1000.0f, "float exponent");

    check(!strconv_to_float("", &v), "float empty string");
    check(!strconv_to_float("1.5x", &v), "float trailing garbage");
    check(!strconv_to_float(NULL, &v), "float null string");
    check(!strconv_to_float("1", NULL), "float null output");
    check(!strconv_to_float("1e39", &v) && errno == ERANGE, "float overflow");
    check(!strconv_to_float("1e-50", &v) && errno == ERANGE, "float underflow");

    v = 99.0f;
    strconv_to_float("nope", &v);
    check(v == 99.0f, "float output preserved on failure");
}

static void test_double(void) {
    double v = 0;

    check(strconv_to_double("0", &v) && v == 0.0, "double zero");
    check(strconv_to_double("1.5", &v) && v == 1.5, "double positive");
    check(strconv_to_double("  -2.25", &v) && v == -2.25,
          "double leading space and sign");
    check(strconv_to_double("1e39", &v) && v == 1e39,
          "double beyond float range");

    check(!strconv_to_double("", &v), "double empty string");
    check(!strconv_to_double("1.5x", &v), "double trailing garbage");
    check(!strconv_to_double(NULL, &v), "double null string");
    check(!strconv_to_double("1", NULL), "double null output");
    check(!strconv_to_double("1e309", &v) && errno == ERANGE,
          "double overflow");
    check(!strconv_to_double("1e-400", &v) && errno == ERANGE,
          "double underflow");

    v = 99.0;
    strconv_to_double("nope", &v);
    check(v == 99.0, "double output preserved on failure");
}

int main(void) {
    test_int32();
    test_uint32();
    test_float();
    test_double();

    printf("%s\n", failures > 0 ? "some tests failed" : "all tests passed");
    return failures > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
