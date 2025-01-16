#pragma once

#define BAIL_IF(x)           \
    do {                     \
        int err = x;         \
        if (err) return err; \
    } while (0)