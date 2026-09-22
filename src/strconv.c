/*
 * strconv.c --- Safe string to number conversions
 *
 * Author: Beckett Lee-Messer
 * Created: 09-22-2026
 * Version: 1.0
 *
 * Description: Implementation of the conversions declared in strconv.h
 *
 */

#include "lib50/strconv.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

/* Check that a strto* call consumed all of str.
 * Sets errno to EINVAL and returns false if it did not.
 */
static bool parsed_whole(const char *str, const char *endp) {
    if (endp == str) {
        // got no digits
        errno = EINVAL;
        return false;
    }

    if (*endp != '\0') {
        // strto* stops early if it encounters a non digit,
        // so not reaching end of str means there is trailing garbage
        errno = EINVAL;
        return false;
    }

    return true;
}

bool strconv_to_int32(const char *str, int32_t *outp) {
    char *endp = NULL;
    long val = 0;

    if (str == NULL || outp == NULL) {
        errno = EINVAL;
        return false;
    }

    errno = 0;
    val = strtol(str, &endp, 10);

    if (!parsed_whole(str, endp)) {
        return false;
    }

    if (errno == ERANGE) {
        // value did not fit in a long
        return false;
    }

    if (val < INT32_MIN || val > INT32_MAX) {
        // long may be 64- or 32-bit depending on platform,
        // so must check/clamp to 32 always to fit return type
        errno = ERANGE;
        return false;
    }

    *outp = (int32_t)val;
    return true;
}

bool strconv_to_uint32(const char *str, uint32_t *outp) {
    const char *p = str;
    char *endp = NULL;
    unsigned long val = 0;

    if (str == NULL || outp == NULL) {
        errno = EINVAL;
        return false;
    }

    // strtoul negates "-1" into a huge value instead of failing,
    // so reject the sign ourselves
    while (isspace((unsigned char)*p)) {
        p++;
    }
    if (*p == '-') {
        errno = EINVAL;
        return false;
    }

    errno = 0;
    val = strtoul(str, &endp, 10);

    if (!parsed_whole(str, endp)) {
        return false;
    }

    if (errno == ERANGE) {
        // value did not fit in an unsigned long
        return false;
    }

    if (val > UINT32_MAX) {
        // unsigned long may be 64- or 32-bit depending on platform
        errno = ERANGE;
        return false;
    }

    *outp = (uint32_t)val;
    return true;
}

bool strconv_to_float(const char *str, float *outp) {
    char *endp = NULL;
    float val = 0;

    if (str == NULL || outp == NULL) {
        errno = EINVAL;
        return false;
    }

    errno = 0;
    val = strtof(str, &endp);

    if (!parsed_whole(str, endp)) {
        return false;
    }

    if (errno == ERANGE) {
        // overflow returns +/-HUGE_VALF, underflow returns a value near 0
        // which triggers errno
        return false;
    }

    *outp = val;
    return true;
}

bool strconv_to_double(const char *str, double *outp) {
    char *endp = NULL;
    double val = 0;

    if (str == NULL || outp == NULL) {
        errno = EINVAL;
        return false;
    }

    errno = 0;
    val = strtod(str, &endp);

    if (!parsed_whole(str, endp)) {
        return false;
    }

    if (errno == ERANGE) {
        // overflow returns +/-HUGE_VAL, underflow returns a value near 0
        // which triggers errno
        return false;
    }

    *outp = val;
    return true;
}
