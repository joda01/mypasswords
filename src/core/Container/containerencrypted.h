///
/// \file       ContainerEncrypted.h
/// \author     Joachim Danmayr
/// \date       2016-10-09
///
/// \brief      Encrypted password container file
///

#ifndef CONTAINERENCRYPTED_H
#define CONTAINERENCRYPTED_H


#include "Container.h"

///
/// \class      ContainerEncrypted
/// \author     Joachim Danmayr
/// \date       2016-10-14
///
/// \brief      Encrypted password container
///
class ContainerEncrypted : public Container
{
public:
    enum Error_t{eNoError, ePasswordWronng, eContainerNotSupported, eCouldNotOpenOrReadFile, eUnhandledError, eSaveError, eArgon2Error};
    ContainerEncrypted();
    virtual ~ContainerEncrypted();
    void GetArgon2Setting(uint32_t& itCost, uint32_t& memCost, uint32_t& lanes);
    std::string GetFileName(){return mFileName;}
    bool IsLocked(){return (mState==eLocked) ? true:false;}
    bool IsOpen(){return (mState==eOpen) ? true:false;}

protected:
    Error_t New(std::string fileName, const char *password, uint32_t passwordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes);
    Error_t Open(std::string fileName, const char *password, uint32_t passwordSize);
    Error_t Save(void);
    Error_t Unlock(const char *password, uint32_t passwordSize);
    Error_t Lock(void);
    Error_t Close(void);
    Error_t ChangePassword(const char *oldPassword, uint32_t oldPasswordSize,const char *newPassword, uint32_t newPasswordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes);

private:

    ///////////////////////////////////////////////////
    /// Enums
    enum ContainerState_t {eClosed, eOpen, eLocked};

    ///////////////////////////////////////////////////
    /// Constants
    static const int32_t KEY_SIZE   = 32;
    static const int32_t BLOCK_SIZE = 32;
    static const int32_t HASH_SIZE  = 32;
    static const int32_t SALT_SIZE  = 32;

    static const uint8_t  CONTAINER_VERSION_V1 = 1;
    static const uint32_t CONTAINER_SIGN = 0x43525950;//'CRYP'
    static const int32_t  HEADERV1_SIZE = SALT_SIZE+28;

    ///////////////////////////////////////////////////
    /// Structs
    typedef union
    {
        char bytes[HEADERV1_SIZE];
        struct{
            uint32_t sign;
            uint32_t headerSize;
            uint32_t containerVersion;
            char salt[SALT_SIZE];
            uint32_t argon2IterationCost;
            uint32_t argon2MemoryCost;
            uint32_t argon2Lanes;
            uint32_t headerCrc;
        }data;
    }HeaderV1_t;

    ///////////////////////////////////////////////////
    /// Methods
    void DeleteAllArrays();
    void DeletePlainDataArray();
    void DeleteEncryptedDataArray();
    void DeleteKey();
    void CreatePasswordHash(const char* password, uint32_t passwordSize, const char* salt, char* hash) const;
    void CreateSessionKey();

    ///////////////////////////////////////////////////
    /// Members
    ContainerState_t mState;
    char* mPlainData;
    char* mEncryptedData;
    std::string mFileName;
    uint32_t mPlainDataSize;
    uint32_t mEncryptedDataSize;
    char     mKey[KEY_SIZE]={0};
    char     mSessionKey[msg::ElementaryEncrypted::MAX_ENCRYPTED_DATA_SIZE]={0};
    HeaderV1_t mHeaderV1;
};

#endif // CONTAINERENCRYPTED_H
