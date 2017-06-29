/**
 * @file	Secure.h
 *
 * Secure functions
 *
 * @author	Joachim Danmayr
 * @date	206-07-16
 *
 */
#ifndef SECURE_H_
#define SECURE_H_

#include <cstdint>
#include <string>

namespace crypto
{
    void* SecureClearArray(char *data, uint32_t size);
    void* SecureClearString(std::string* data);

    bool LockMemory(char* data, uint32_t size);
    bool UnLockMemory(char* data, uint32_t size);
    bool StartSecureDesktop();
    bool CloseSecureDesktop();
}

#endif
