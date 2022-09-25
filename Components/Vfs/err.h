#ifndef __ERR_H__
#define __ERR_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int err_t;

#define OK 0
#define FAIL -1
#define ERR_INVALID_ARG 100
#define ERR_NO_MEM 101

#ifdef __cplusplus
}
#endif

#endif