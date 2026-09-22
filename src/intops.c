/*
 * intops.c --- Safe integer conversions
 *
 * Author: Beckett Lee-Messer
 * Created: 09-22-2026
 * Version: 1.0
 *
 * Description: Implementation of the conversions declared in intops.h
 *
 */

#include "lib50/intops.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

bool strtoint32(const char *str, int32_t *outp) {
    char *endp = NULL;
    long val = 0;

    if (str == NULL || outp == NULL) {
        errno = EINVAL;
        return false;
    }

    errno = 0;
    val = strtol(str, &endp, 10);

    if (endp == str) {
        // got no digits
        errno = EINVAL;
        return false;
    }

    if (*endp != '\0') {
        // strtol stops early if it encounters non digit,
        // so not reaching end of str means there is trailing garbage
        errno = EINVAL;
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
