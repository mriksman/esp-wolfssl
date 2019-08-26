/* sp_int.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */


#ifndef WOLF_CRYPT_SP_INT_H
#define WOLF_CRYPT_SP_INT_H

#include <stdint.h>
#include <limits.h>

/* Make sure WOLFSSL_SP_ASM build option defined when requested */
#if !defined(WOLFSSL_SP_ASM) && ( \
      defined(WOLFSSL_SP_X86_64_ASM) || defined(WOLFSSL_SP_ARM32_ASM) || \
      defined(WOLFSSL_SP_ARM64_ASM)  || defined(WOLFSSL_SP_ARM_THUMB_ASM) || \
      defined(WOLFSSL_SP_ARM_CORTEX_M_ASM))
    #define WOLFSSL_SP_ASM
#endif


#ifdef WOLFSSL_SP_X86_64_ASM
    #define SP_WORD_SIZE 64

    #define HAVE_INTEL_AVX1
    #define HAVE_INTEL_AVX2
#elif defined(WOLFSSL_SP_ARM64_ASM)
    #define SP_WORD_SIZE 64
#elif defined(WOLFSSL_SP_ARM32_ASM)
    #define SP_WORD_SIZE 32
#elif defined(WOLFSSL_SP_ARM_THUMB_ASM)
    #define SP_WORD_SIZE 32
#endif

#ifndef SP_WORD_SIZE
    #if defined(NO_64BIT) || !defined(HAVE___UINT128_T)
        #define SP_WORD_SIZE 32
    #else
        #define SP_WORD_SIZE 64
    #endif
#endif

#ifndef WOLFSSL_SP_ASM
  #if SP_WORD_SIZE == 32
    typedef int32_t sp_digit;
    typedef uint32_t sp_int_digit;
  #elif SP_WORD_SIZE == 64
    typedef int64_t sp_digit;
    typedef uint64_t sp_int_digit;
    typedef unsigned long uint128_t __attribute__ ((mode(TI)));
    typedef long int128_t __attribute__ ((mode(TI)));
  #else
    #error Word size not defined
  #endif
#else
  #if SP_WORD_SIZE == 32
    typedef uint32_t sp_digit;
    typedef uint32_t sp_int_digit;
  #elif SP_WORD_SIZE == 64
    typedef uint64_t sp_digit;
    typedef uint64_t sp_int_digit;
    typedef unsigned long uint128_t __attribute__ ((mode(TI)));
    typedef long int128_t __attribute__ ((mode(TI)));
  #else
    #error Word size not defined
  #endif
#endif

#ifdef WOLFSSL_SP_MATH
#include <wolfssl/wolfcrypt/random.h>

#if !defined(WOLFSSL_HAVE_SP_RSA) && !defined(WOLFSSL_HAVE_SP_DH)
    #if !defined(NO_PWDBASED) && defined(WOLFSSL_SHA512)
        #define SP_INT_DIGITS        ((512 + SP_WORD_SIZE) / SP_WORD_SIZE)
    #else
        #define SP_INT_DIGITS        ((256 + SP_WORD_SIZE) / SP_WORD_SIZE)
    #endif
#elif defined(WOLFSSL_SP_NO_3072)
    #define SP_INT_DIGITS        ((2048 + SP_WORD_SIZE) / SP_WORD_SIZE)
#else
    #define SP_INT_DIGITS        ((3072 + SP_WORD_SIZE) / SP_WORD_SIZE)
#endif

#define sp_isodd(a) (a->used != 0 && (a->dp[0] & 1))

#ifdef HAVE_WOLF_BIGINT
    /* raw big integer */
    typedef struct WC_BIGINT {
        byte*   buf;
        word32  len;
        void*   heap;
    } WC_BIGINT;
    #define WOLF_BIGINT_DEFINED
#endif

typedef struct sp_int {
    int used;
    int size;
    sp_int_digit dp[SP_INT_DIGITS];
#ifdef HAVE_WOLF_BIGINT
    struct WC_BIGINT raw; /* unsigned binary (big endian) */
#endif
} sp_int;

typedef sp_int mp_int;
typedef sp_digit mp_digit;

#include <wolfssl/wolfcrypt/wolfmath.h>


MP_API int sp_init(sp_int* a);
MP_API int sp_init_multi(sp_int* a, sp_int* b, sp_int* c, sp_int* d,
                         sp_int* e, sp_int* f);
MP_API void sp_clear(sp_int* a);
MP_API int sp_unsigned_bin_size(sp_int* a);
MP_API int sp_read_unsigned_bin(sp_int* a, const byte* in, word32 inSz);
MP_API int sp_read_radix(sp_int* a, const char* in, int radix);
MP_API int sp_cmp(sp_int* a, sp_int* b);
MP_API int sp_count_bits(sp_int* a);
MP_API int sp_leading_bit(sp_int* a);
MP_API int sp_to_unsigned_bin(sp_int* a, byte* out);
MP_API int sp_to_unsigned_bin_len(sp_int* a, byte* out, int outSz);
MP_API void sp_forcezero(sp_int* a);
MP_API int sp_copy(sp_int* a, sp_int* b);
MP_API int sp_set(sp_int* a, sp_int_digit d);
MP_API int sp_iszero(sp_int* a);
MP_API void sp_clamp(sp_int* a);
MP_API int sp_grow(sp_int* a, int l);
MP_API int sp_sub_d(sp_int* a, sp_int_digit d, sp_int* r);
MP_API int sp_cmp_d(sp_int* a, sp_int_digit d);
MP_API int sp_mod(sp_int* a, sp_int* m, sp_int* r);
MP_API void sp_zero(sp_int* a);
MP_API int sp_add_d(sp_int* a, sp_int_digit d, sp_int* r);
MP_API int sp_lshd(sp_int* a, int s);
MP_API int sp_add(sp_int* a, sp_int* b, sp_int* r);
MP_API int sp_set_int(sp_int* a, unsigned long b);
MP_API int sp_tohex(sp_int* a, char* str);


#define MP_OKAY    0
#define MP_NO      0
#define MP_YES     1

#define MP_RADIX_HEX     16

#define MP_GT    1
#define MP_EQ    0
#define MP_LT    -1

#define MP_MEM   -2
#define MP_VAL   -3

#define DIGIT_BIT  SP_WORD_SIZE

#define CheckFastMathSettings() 1

#define mp_free(a)

#define mp_init                     sp_init
#define mp_init_multi               sp_init_multi
#define mp_clear                    sp_clear
#define mp_read_unsigned_bin        sp_read_unsigned_bin
#define mp_unsigned_bin_size        sp_unsigned_bin_size
#define mp_read_radix               sp_read_radix
#define mp_cmp                      sp_cmp
#define mp_count_bits               sp_count_bits
#define mp_leading_bit              sp_leading_bit
#define mp_to_unsigned_bin          sp_to_unsigned_bin
#define mp_to_unsigned_bin_len      sp_to_unsigned_bin_len
#define mp_forcezero                sp_forcezero
#define mp_copy                     sp_copy
#define mp_set                      sp_set
#define mp_iszero                   sp_iszero
#define mp_clamp                    sp_clamp
#define mp_grow                     sp_grow
#define mp_sub_d                    sp_sub_d
#define mp_cmp_d                    sp_cmp_d
#define mp_mod                      sp_mod
#define mp_zero                     sp_zero
#define mp_add_d                    sp_add_d
#define mp_lshd                     sp_lshd
#define mp_add                      sp_add
#define mp_isodd                    sp_isodd
#define mp_set_int                  sp_set_int
#define mp_tohex                    sp_tohex

#endif

#endif /* WOLF_CRYPT_SP_H */
