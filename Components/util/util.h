#ifndef __UTIL_H
#define __UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define CONCAT_2(p1, p2) p1##p2

#define IS_POWER_OF_TWO(A) (((A) != 0) && (((( A )-1) & (A)) == 0))
#ifdef __cplusplus
}
#endif

#endif