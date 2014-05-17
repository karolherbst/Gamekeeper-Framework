/*
 * Copyright (c) 2012 Rogerz Zhang <rogerz.zhang@gmail.com>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <string.h>
#include <assert.h>

#include <jansson.h>
#include "jansson_path.h"


json_t *json_path_get(const json_t *json, const char *path)
{
    static const char root_chr = '$', array_open = '[';
    static const char *path_delims = ".[", *array_close = "]";
    const json_t *cursor;
    char *token, *buf, *peek, *endptr, delim = '\0';
    const char *expect;

    if (!json || !path || path[0] != root_chr)
        return NULL;
    else
        buf = strdup(path);

    peek = buf + 1;
    cursor = json;
    token = NULL;
    expect = path_delims;

    while (peek && *peek && cursor)
    {
        char *last_peek = peek;
        peek = strpbrk(peek, expect);
        if (peek) {
            if (!token && peek != last_peek)
                goto fail;
            delim = *peek;
            *peek++ = '\0';
        } else if (expect != path_delims || !token) {
            goto fail;
        }

        if (expect == path_delims) {
            if (token) {
                cursor = json_object_get(cursor, token);
            }
            expect = (delim == array_open ? array_close : path_delims);
            token = peek;
        } else if (expect == array_close) {
            size_t index = strtol(token, &endptr, 0);
            if (*endptr)
                goto fail;
            cursor = json_array_get(cursor, index);
            token = NULL;
            expect = path_delims;
        } else {
            goto fail;
        }
    }

    free(buf);
    return (json_t *)cursor;
fail:
    free(buf);
    return NULL;
}
