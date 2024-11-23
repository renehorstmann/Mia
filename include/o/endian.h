#ifndef O_ENDIAN_H
#define O_ENDIAN_H

/**
 * @file endian.h
 *
 * Host to little or big endian and vice versa for 16, 32 and 64 bit types.
 *
 * @note namespace for these name is either o_endian_ or oX_endian_ (X = primitive like ou32)
 */

#include "common.h"

/** @return true if system is little endian */
O_INLINE
bool o_endian_system_is_binary_little_endian(void) {
    int n = 1;
    // little endian if true
    return *(char *) &n == 1;
}

/**
 * Swaps byte order of the given buf
 * @param buf number or something to swap the endian of
 * @param n maximal 256 bits
 */
O_INLINE
void o_endian_swap(void *buf, int n) {
    assert(n <= 256 && "o_endian_swap: max 256 bits");
    char tmp[256];
    char *buf_data = (char *) buf;
    for (int i = 0; i < n; i++)
        tmp[i] = buf_data[n - i - 1];
    o_memcpy(buf_data, tmp, 1, n);
}

//
// Contents:
// implementation for unsigned types:
// ouX to le|be
// ouX from le|be
//
// wrapper for signed, etc:
// oiX, osize, of32, of64
//



//
// unsigned (implementations)
//

/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
ou16 ou16_endian_to_big(ou16 x) {
    ou16 result;
    ou8 *c;
    c = (ou8 *) &result;
    c[0] = (x >> 8) & 0xFF;
    c[1] = x & 0xFF;
    return result;
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
ou16 ou16_endian_to_lil(ou16 x) {
    ou16 result;
    ou8 *c;
    c = (ou8 *) &result;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
ou16 ou16_endian_from_lil(ou16 x) {
    ou16 result;
    ou8 *c;

    c = (ou8 *) &x;
    result = (ou16) c[0];
    result |= (ou16) c[1] << 8;
    return result;
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
ou16 ou16_endian_from_big(ou16 x) {
    ou16 result;
    ou8 *c;

    c = (ou8 *) &x;
    result = (ou16) c[1];
    result |= (ou16) c[0] << 8;
    return result;
}



/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
ou32 ou32_endian_to_big(ou32 x) {
    ou32 result;
    ou8 *c;
    c = (ou8 *) &result;
    c[0] = (x >> 24) & 0xFF;
    c[1] = (x >> 16) & 0xFF;
    c[2] = (x >> 8) & 0xFF;
    c[3] = x & 0xFF;
    return result;
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
ou32 ou32_endian_to_lil(ou32 x) {
    ou32 result;
    ou8 *c;
    c = (ou8 *) &result;
    c[3] = (x >> 24) & 0xFF;
    c[2] = (x >> 16) & 0xFF;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
ou32 ou32_endian_from_lil(ou32 x) {
    ou32 result;
    ou8 *c;

    c = (ou8 *) &x;
    result = (ou32) c[0];
    result |= (ou32) c[1] << 8;
    result |= (ou32) c[2] << 16;
    result |= (ou32) c[3] << 24;
    return result;
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
ou32 ou32_endian_from_big(ou32 x) {
    ou32 result;
    ou8 *c;

    c = (ou8 *) &x;
    result = (ou32) c[3];
    result |= (ou32) c[2] << 8;
    result |= (ou32) c[1] << 16;
    result |= (ou32) c[0] << 24;
    return result;
}



/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
ou64 ou64_endian_to_big(ou64 x) {
    ou64 result;
    ou8 *c;
    c = (ou8 *) &result;
    c[0] = x >> 56;
    c[1] = (x >> 48) & 0xFF;
    c[2] = (x >> 40) & 0xFF;
    c[3] = (x >> 32) & 0xFF;
    c[4] = (x >> 24) & 0xFF;
    c[5] = (x >> 16) & 0xFF;
    c[6] = (x >> 8) & 0xFF;
    c[7] = x & 0xFF;
    return result;
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
ou64 ou64_endian_to_lil(ou64 x) {
    ou64 result;
    ou8 *c;
    c = (ou8 *) &result;
    c[7] = x >> 56;
    c[6] = (x >> 48) & 0xFF;
    c[5] = (x >> 40) & 0xFF;
    c[4] = (x >> 32) & 0xFF;
    c[3] = (x >> 24) & 0xFF;
    c[2] = (x >> 16) & 0xFF;
    c[1] = (x >> 8) & 0xFF;
    c[0] = x & 0xFF;
    return result;
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
ou64 ou64_endian_from_lil(ou64 x) {
    ou64 result;
    ou8 *c;

    c = (ou8 *) &x;
    result = (ou64) c[0];
    result |= (ou64) c[1] << 8;
    result |= (ou64) c[2] << 16;
    result |= (ou64) c[3] << 24;
    result |= (ou64) c[4] << 32;
    result |= (ou64) c[5] << 40;
    result |= (ou64) c[6] << 48;
    result |= (ou64) c[7] << 56;
    return result;
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
ou64 ou64_endian_from_big(ou64 x) {
    ou64 result;
    ou8 *c;

    c = (ou8 *) &x;
    result = (ou64) c[7];
    result |= (ou64) c[6] << 8;
    result |= (ou64) c[5] << 16;
    result |= (ou64) c[4] << 24;
    result |= (ou64) c[3] << 32;
    result |= (ou64) c[2] << 40;
    result |= (ou64) c[1] << 48;
    result |= (ou64) c[0] << 56;
    return result;
}


//
// signed wrapper
//

/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
oi16 oi16_endian_to_big(oi16 x) {
    ou16 ret = ou16_endian_to_big(*((ou16*)&x));
    return *((oi16*)&ret);
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
oi16 oi16_endian_to_lil(oi16 x) {
    ou16 ret = ou16_endian_to_lil(*((ou16*)&x));
    return *((oi16*)&ret);
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
oi16 oi16_endian_from_lil(oi16 x) {
    ou16 ret = ou16_endian_from_lil(*((ou16*)&x));
    return *((oi16*)&ret);
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
oi16 oi16_endian_from_big(oi16 x) {
    ou16 ret = ou16_endian_from_big(*((ou16*)&x));
    return *((oi16*)&ret);
}



/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
oi32 oi32_endian_to_big(oi32 x) {
    ou32 ret = ou32_endian_to_big(*((ou32*)&x));
    return *((oi32*)&ret);
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
oi32 oi32_endian_to_lil(oi32 x) {
    ou32 ret = ou32_endian_to_lil(*((ou32*)&x));
    return *((oi32*)&ret);
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
oi32 oi32_endian_from_lil(oi32 x) {
    ou32 ret = ou32_endian_from_lil(*((ou32*)&x));
    return *((oi32*)&ret);
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
oi32 oi32_endian_from_big(oi32 x) {
    ou32 ret = ou32_endian_from_big(*((ou32*)&x));
    return *((oi32*)&ret);
}



/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
oi64 oi64_endian_to_big(oi64 x) {
    ou64 ret = ou64_endian_to_big(*((ou64*)&x));
    return *((oi64*)&ret);
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
oi64 oi64_endian_to_lil(oi64 x) {
    ou64 ret = ou64_endian_to_lil(*((ou64*)&x));
    return *((oi64*)&ret);
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
oi64 oi64_endian_from_lil(oi64 x) {
    ou64 ret = ou64_endian_from_lil(*((ou64*)&x));
    return *((oi64*)&ret);
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
oi64 oi64_endian_from_big(oi64 x) {
    ou64 ret = ou64_endian_from_big(*((ou64*)&x));
    return *((oi64*)&ret);
}


//
// float wrapper (of32, of64)
//


/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
of32 of32_endian_to_big(of32 x) {
    void *x_ptr = &x;
    ou32 ret = ou32_endian_to_big(*((ou32*) x_ptr));
    void *ret_ptr = &ret;
    return *((of32*) ret_ptr);
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
of32 of32_endian_to_lil(of32 x) {
    void *x_ptr = &x;
    ou32 ret = ou32_endian_to_lil(*((ou32*) x_ptr));
    void *ret_ptr = &ret;
    return *((of32*) ret_ptr);
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
of32 of32_endian_from_lil(of32 x) {
    void *x_ptr = &x;
    ou32 ret = ou32_endian_from_lil(*((ou32*) x_ptr));
    void *ret_ptr = &ret;
    return *((of32*) ret_ptr);
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
of32 of32_endian_from_big(of32 x) {
    void *x_ptr = &x;
    ou32 ret = ou32_endian_from_big(*((ou32*) x_ptr));
    void *ret_ptr = &ret;
    return *((of32*) ret_ptr);
}



/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
O_INLINE
of64 of64_endian_to_big(of64 x) {
    void *x_ptr = &x;
    ou64 ret = ou64_endian_to_big(*((ou64*) x_ptr));
    void *ret_ptr = &ret;
    return *((of64*) ret_ptr);
}

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
O_INLINE
of64 of64_endian_to_lil(of64 x) {
    void *x_ptr = &x;
    ou64 ret = ou64_endian_to_lil(*((ou64*) x_ptr));
    void *ret_ptr = &ret;
    return *((of64*) ret_ptr);
}

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
O_INLINE
of64 of64_endian_from_lil(of64 x) {
    void *x_ptr = &x;
    ou64 ret = ou64_endian_from_lil(*((ou64*) x_ptr));
    void *ret_ptr = &ret;
    return *((of64*) ret_ptr);
}

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
O_INLINE
of64 of64_endian_from_big(of64 x) {
    void *x_ptr = &x;
    ou64 ret = ou64_endian_from_big(*((ou64*) x_ptr));
    void *ret_ptr = &ret;
    return *((of64*) ret_ptr);
}


//
// osize wrapper (==oi64)
//


/**
 * Swaps host into big endian
 * @param x host value
 * @return x as big endian
 */
#define osize_endian_to_big(x) oi64_endian_to_big(x)

/**
 * Swaps host into little endian
 * @param x host value
 * @return x as little endian
 */
#define osize_endian_to_lil(x) oi64_endian_to_lil(x)

/**
 * Swaps from little endian into host
 * @param x little endian value
 * @return x in host endian
 */
#define osize_endian_from_lil(x) oi64_endian_from_lil(x)

/**
 * Swaps from big endian into host
 * @param x big endian value
 * @return x in host endian
 */
#define osize_endian_from_big(x) oi64_endian_from_big(x)



#endif //O_ENDIAN_H
