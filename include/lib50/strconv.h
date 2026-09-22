/*
 * strconv.h --- Safe string to number conversions
 *
 * Author: Beckett Lee-Messer
 * Created: 09-22-2026
 * Version: 1.0
 *
 * Description: Converts from string to integer or floating point while
 *              reporting failures clearly. Wraps strtol, strtoul, strtof
 *              and strtod.
 *
 * All functions follow this contract:
 *
 * Arguments:
 * str: the null-terminated string to convert; optional leading whitespace is
 *      accepted, trailing characters are not.
 *
 * outp: where to store the result, written only when the call succeeds
 *
 * Returns:
 * true on success.
 *
 * false on failure, with errno set:
 *     EINVAL if str or outp is NULL, if str holds no number or if characters
 *            follow the number
 *     ERANGE if the value does not fit in the output type.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Convert a decimal string to a 32-bit signed integer.
 * A leading + or - is accepted.
 */
bool strconv_to_int32(const char *str, int32_t *outp);

/* Convert a decimal string to a 32-bit unsigned integer.
 * A leading + is accepted. A leading '-' fails with EINVAL, since strtoul
 * would otherwise wrap "-1" to UINT32_MAX.
 */
bool strconv_to_uint32(const char *str, uint32_t *outp);

/* Convert a string to a float.
 * Accepts anything strtof accepts, including exponents, hex floats, "inf"
 * and "nan". Values too large or too small to represent as a normal float
 * fail with ERANGE.
 */
bool strconv_to_float(const char *str, float *outp);

/* Convert a string to a double.
 * Accepts anything strtod accepts, including exponents, hex floats, "inf"
 * and "nan". Values too large or too small to represent as a normal double
 * fail with ERANGE.
 */
bool strconv_to_double(const char *str, double *outp);
