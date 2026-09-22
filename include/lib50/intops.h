/*
 * intops.h --- Safe integer conversions
 *
 * Author: Beckett Lee-Messer
 * Created: 09-22-2026
 * Version: 1.0
 *
 * Description: Converts from string to integer while reporting failures clearly.
 *
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Convert a decimal string to a 32-bit signed integer.
 *
 * Arguments:
 * str: the null-terminated string to convert; optional leading whitespace and
 *      a leading + or - are accepted, trailing characters are not.
 *
 * outp: where to store the result, written only when the call succeeds
 *
 * Returns:
 * true on success.
 *
 * false on failure, with errno set:
 *     EINVAL if str or outp is NULL, if str holds no digits or if characters follow the number
 *     ERANGE if the value does not fit in an int32_t.
 */
bool strtoint32(const char *str, int32_t *outp);
