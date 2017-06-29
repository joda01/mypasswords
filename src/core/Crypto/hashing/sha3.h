/**
 * @file	Sha3.H
 *
 * @author	Joachim Danmayr
 * @date	206-07-16
 *
 */

#ifndef RHASH_SHA3_H_
#define RHASH_SHA3_H_

#include <cstdint>
#include "ustd.h"
#include "hashing.h"

namespace crypto {
class Sha3 : public Hashing
{
public:
    Sha3();
    virtual Error_t Generate(const char* dataIn, uint32_t dataSize, char* hashOut,HashSize_t hashSize) override;
private:
    static const uint32_t sha3_max_permutation_size = 25;
    static const uint32_t sha3_max_rate_in_qwords = 24;

    typedef struct sha3_ctx
    {
        uint64_t hash[sha3_max_permutation_size];    /** 1600 bits algorithm hashing state */
        uint64_t message[sha3_max_rate_in_qwords];   /** 1536-bit buffer for leftovers */
        unsigned rest;                               /** count of bytes in the message[] buffer */
        unsigned block_size;                         /** size of a message block processed at once */
    } sha3_ctx;

    void rhash_sha3_update(sha3_ctx *ctx, const unsigned char* msg, size_t size);
    void rhash_sha3_final(sha3_ctx *ctx, unsigned char* result);
    void rhash_keccak_init(sha3_ctx *ctx, unsigned bits);
    void keccak_theta(uint64_t *A);
    void keccak_pi(uint64_t *A);
    void keccak_chi(uint64_t *A);
    void rhash_sha3_permutation(uint64_t *state);
    void rhash_sha3_process_block(uint64_t hash[25], const uint64_t *block, size_t block_size);

    sha3_ctx mCtx;

#ifdef USE_KECCAK
#define rhash_keccak_224_init rhash_sha3_224_init
#define rhash_keccak_256_init rhash_sha3_256_init
#define rhash_keccak_384_init rhash_sha3_384_init
#define rhash_keccak_512_init rhash_sha3_512_init
#define rhash_keccak_update rhash_sha3_update
    void rhash_keccak_final(sha3_ctx *ctx, unsigned char* result);
#endif
};
}
#endif /* RHASH_SHA3_H */
