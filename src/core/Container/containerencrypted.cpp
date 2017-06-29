///
/// \file       ContainerEncrypted.cpp
/// \author     Joachim Danmayr
/// \date       2016-10-09
///
/// \brief      Encrypted password container file
///

#include "containerencrypted.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../Crypto/encryption/Rijndael.h"
#include "../Crypto/hashing/sha3.h"
#include "../Crypto/Secure.h"
#include "../Crypto/RandomNumber.h"
#include "../Crypto/hashing/argon/argon2.h"

///
/// \brief      Constructor
/// \author     Joachim Danmayr
/// \date       2016-10-09
///
ContainerEncrypted::ContainerEncrypted():
    Container(mSessionKey),
    mPlainData(0),
    mEncryptedData(0),
    mPlainDataSize(0),
    mEncryptedDataSize(0),
    mState(eClosed)
{
    bool ok = crypto::LockMemory(mKey,KEY_SIZE);
    ok |= crypto::LockMemory(mSessionKey,sizeof(mSessionKey));
    if(false==ok){
        std::cout << "ContainerEncrypted::ContainerEncrypted::Memory locking faild!" << std::endl;
    }
}

///
/// \brief      Destructor
/// \author     Joachim Danmayr
/// \date       2016-10-09
///
ContainerEncrypted::~ContainerEncrypted()
{
    Close();
    DeleteAllArrays();
    DeleteKey();
    std::cout << "ContainerEncrypted::~ContainerEncrypted: Destructor executed" << std::endl;
}

///
/// \brief      Free memory of all arrays in this class
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void ContainerEncrypted::DeleteAllArrays()
{
    DeletePlainDataArray();
    DeleteEncryptedDataArray();
}

///
/// \brief      Delete encryption key and session key
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void ContainerEncrypted::DeleteKey()
{
    crypto::SecureClearArray(mSessionKey,sizeof(mSessionKey));
    crypto::SecureClearArray(mKey,KEY_SIZE);
}

///
/// \brief      Free memory of the plain data array
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void ContainerEncrypted::DeletePlainDataArray()
{
    if(0!=mPlainData){
        crypto::SecureClearArray(mPlainData,mPlainDataSize);
        delete mPlainData;
        mPlainDataSize = 0;
        mPlainData = 0;
    }
}

///
/// \brief      Free memory of the encrypted data array
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void ContainerEncrypted::DeleteEncryptedDataArray()
{
    if(0!=mEncryptedData){
        crypto::SecureClearArray(mEncryptedData,mEncryptedDataSize);
        delete mEncryptedData;
        mEncryptedDataSize = 0;
        mEncryptedData = 0;
    }
}


///
/// \brief      Locks an opened database
/// \author     Joachim Danmayr
/// \date       2016-11-05
///
/// \return     0 if everything was ok
///
ContainerEncrypted::Error_t ContainerEncrypted::Lock(void)
{
    if(eOpen == mState){
        Save();
        DeleteKey();
        DeleteAllArrays();
        Clear();
        ResetFilter();
        std::cout << "ContainerEncrypted::Lock: Container locked!" << std::endl;
        mState = eLocked;
    }
    return eNoError;
}


///
/// \brief      Close container file
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
/// \return     0 if everything was ok
///
ContainerEncrypted::Error_t ContainerEncrypted::Close(void)
{
    mFileName.clear();
    DeleteKey();
    DeleteAllArrays();
    Clear();
    ResetFilter();
    std::cout << "ContainerEncrypted::Close: Container closed!" << std::endl;
    mState = eClosed;
    return eNoError;
}


///
/// \brief      Creates a new password container
///             - A random salt will be generated
///             - With password and salt a key will be generated
///
/// \author     Joachim Danmayr
/// \date       2016-10-29
///
/// \param[in]  fileName      Filename of the container
/// \param[in]  password      Pointer to the password
/// \param[in]  passwordSize  Number of bytes the password has got
/// \param[in]  argon2ItCost  Iteration cost fir password hashing
/// \param[in]  argon2MemCost Memory cost for password hashing [kB]
/// \param[in]  argon2Lanes   Number of lanes for password hashing
/// \return     0 if everything was ok
///
ContainerEncrypted::Error_t ContainerEncrypted::New(std::string fileName, const char *password, uint32_t passwordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes)
{
    Error_t error = eNoError;
    Close();
    mFileName = fileName;
    mHeaderV1.data.sign=CONTAINER_SIGN;
    mHeaderV1.data.headerSize=HEADERV1_SIZE;
    mHeaderV1.data.containerVersion=CONTAINER_VERSION_V1;
    RandomNumber::Generate(mHeaderV1.data.salt,SALT_SIZE);
    mHeaderV1.data.argon2IterationCost=argon2ItCost;
    mHeaderV1.data.argon2MemoryCost=argon2MemCost;
    mHeaderV1.data.argon2Lanes=argon2Lanes;
    mHeaderV1.data.headerCrc=0;
    CreatePasswordHash(password,passwordSize,mHeaderV1.data.salt,mKey);
    error = Save();
    Close();
    return error;
}

///
/// \brief      Encrypt and save password file
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
/// \param[in]  fileName     Filename of the container
/// \return     0 if everything was ok
///
ContainerEncrypted::Error_t ContainerEncrypted::Save(void)
{
    if(mState==eLocked){
        std::cout << "ContainerEncrypted::Save: Container locked!" << std::endl;
        return eSaveError;
    }
    DeleteAllArrays();

    uint32_t dataSize = GetSize()+GetHeaderSize();
    uint32_t dataSizeNorm = (dataSize+HASH_SIZE) +(BLOCK_SIZE-((dataSize+HASH_SIZE)%BLOCK_SIZE));

    try{
        ///////////////////////////////////////////////////
        /// Serialize data
        mPlainData = new char[dataSizeNorm];
        mPlainDataSize = dataSizeNorm;
        crypto::SecureClearArray(mPlainData,dataSizeNorm);
        std::stringstream stream;
        stream.rdbuf()->pubsetbuf(mPlainData, dataSizeNorm);
        this->Serialize(stream);
        stream.flush();

        ///////////////////////////////////////////////////
        /// Calc SH3 hash
        char sh3Hash[HASH_SIZE]={0};
        crypto::Sha3 sha;
        sha.Generate(mPlainData,(dataSizeNorm-HASH_SIZE),sh3Hash,crypto::Hashing::hash256);

        ///////////////////////////////////////////////////
        /// Write Sha3 to the end of the file
        char* ptrToWriteSha3 = mPlainData+dataSizeNorm-HASH_SIZE;
        std::memcpy(ptrToWriteSha3,sh3Hash,HASH_SIZE);

        ///////////////////////////////////////////////////
        /// Encrypt data
        mEncryptedData = new char[dataSizeNorm];
        mEncryptedDataSize = dataSizeNorm;
        char chain[KEY_SIZE]={0};
        CRijndael aes(mKey,chain,KEY_SIZE,BLOCK_SIZE,CRijndael::CBC);
        aes.Encrypt(mPlainData,mEncryptedData,dataSizeNorm);
        DeletePlainDataArray();

        ///////////////////////////////////////////////////
        /// Save data to file
        std::ofstream outFile (mFileName, std::ofstream::out | std::ios::binary);
        outFile.write(mHeaderV1.bytes,mHeaderV1.data.headerSize);
        outFile.write(mEncryptedData,dataSizeNorm);
        outFile.close();
        std::cout << "ContainerEncrypted::Save: Container saved!" << std::endl;
    }
    catch (std::ofstream::failure error) {
        std::cout << "ContainerEncrypted::Save::ofstream::Error: " << std::endl;
        return eSaveError;
    }
    catch(...){
        std::cout << "ContainerEncrypted::Save::UnhandledError::Error: " << std::endl;
        return eUnhandledError;
    }
    return eNoError;
}

///
/// \brief      Unlock an open container
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
/// \param[in]  password     Password to open the container
/// \return     0 if everything was ok
///
ContainerEncrypted::Error_t ContainerEncrypted::Unlock(const char *password, uint32_t passwordSize)
{
    return Open(mFileName,password,passwordSize);
}


///
/// \brief      Encrypt and save password file
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
/// \param[in]  fileName     Filename of the container
/// \param[in]  password     Password to open the container
/// \return     0 if everything was ok
///
ContainerEncrypted::Error_t ContainerEncrypted::Open(std::string fileName, const char* password, uint32_t passwordSize)
{
    Error_t error = eNoError;
    DeleteAllArrays();
    DeleteKey();
    mFileName = fileName;
    try{
        ///////////////////////////////////////////////////
        /// Read data from file
        std::ifstream ifs (fileName, std::ifstream::in | std::ios::binary);
        ifs.seekg(0,ios_base::end);
        uint32_t fileSize = ifs.tellg();
        if(fileSize>0){
            ifs.seekg(0,ios_base::beg);
            mEncryptedData = new char[fileSize];
            mEncryptedDataSize = fileSize;
            ifs.readsome(mEncryptedData,fileSize);
            ifs.close();


            ///////////////////////////////////////////////////
            /// Read header
            std::memcpy(mHeaderV1.bytes,mEncryptedData,HEADERV1_SIZE);

            switch (mHeaderV1.data.containerVersion) {
            case CONTAINER_VERSION_V1:
                break;
            default:
                throw eContainerNotSupported;
                break;
            }


            ///////////////////////////////////////////////////
            /// Generate key to encrypt
            CreatePasswordHash(password,passwordSize,mHeaderV1.data.salt,mKey);

            ///////////////////////////////////////////////////
            /// Decrypt data
            uint32_t dataSize = fileSize - mHeaderV1.data.headerSize;
            char* encrypteddDataStart = mEncryptedData+mHeaderV1.data.headerSize;
            mPlainData = new char[dataSize];
            mPlainDataSize = dataSize;
            char chain[KEY_SIZE]={0};
            CRijndael aes(mKey,chain,KEY_SIZE,BLOCK_SIZE,CRijndael::CBC);
            aes.Decrypt(encrypteddDataStart,mPlainData,dataSize);
            DeleteEncryptedDataArray();

            ///////////////////////////////////////////////////
            /// Calc SH3 from encrypted data
            char sh3Hash[HASH_SIZE]={0};
            crypto::Sha3 sha;
            sha.Generate(mPlainData,(dataSize-HASH_SIZE),sh3Hash,crypto::Hashing::hash256);
#ifdef DEBUG_MODE
            std::cout << "SH3: "; int n=0;while(n<HASH_SIZE){printf("%02x", sh3Hash[n]);n++;} std::cout << std::endl;
#endif
            ///////////////////////////////////////////////////
            /// Get SH3 from encrypted data and compare
            char* ptrToReadSha3 = mPlainData+dataSize-HASH_SIZE;
            int32_t compResult = std::memcmp(sh3Hash,ptrToReadSha3,HASH_SIZE);

            if(0==compResult){

                ///////////////////////////////////////////////////
                /// Generate a session key for in RAM encryption
                CreateSessionKey();

                ///////////////////////////////////////////////////
                /// Deserialize data because Hash was ok
                std::stringstream stream;
                stream.rdbuf()->pubsetbuf(mPlainData, dataSize);
                this->Deserialize(stream);
                stream.flush();
                mState = eOpen;
                std::cout << "ContainerEncrypted::Save: Container opened!" << std::endl;
            }
            else{
                throw ePasswordWronng;
            }
            DeleteAllArrays();
        }
        else{
            throw eCouldNotOpenOrReadFile;
        }

    }
    catch(ContainerEncrypted::Error_t caughtError){
        DeleteKey();
        DeleteAllArrays();
        error = caughtError;
        std::cout << "ContainerEncrypted::Open::Error: " << caughtError << std::endl;
        return error;

    }
    catch (std::ifstream::failure error) {
        DeleteKey();
        DeleteAllArrays();
        std::cout << "ContainerEncrypted::Open::ifstream::Error: " << std::endl;
        return eCouldNotOpenOrReadFile;
    }
    catch(...){
        std::cout << "ContainerEncrypted::Open::UnhandledError::Error: " << std::endl;
        return eUnhandledError;
    }

    return error;

}

///
/// \brief      Changes the password of an open container
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
/// \param[in]  password     Password
/// \param[in]  passwordSize Number of data bytes of the password
/// \param[in]  argon2ItCost  Iteration cost fir password hashing
/// \param[in]  argon2MemCost Memory cost for password hashing [kB]
/// \param[in]  argon2Lanes   Number of lanes for password hashing
///
ContainerEncrypted::Error_t ContainerEncrypted::ChangePassword(const char *oldPassword, uint32_t oldPasswordSize,const char *newPassword, uint32_t newPasswordSize, uint32_t argon2ItCost, uint32_t argon2MemCost, uint32_t argon2Lanes)
{
    // Check if old password matches with password of the actual container
    char key[KEY_SIZE]={0};
    CreatePasswordHash(oldPassword, oldPasswordSize, mHeaderV1.data.salt,key);
    int32_t compResult = std::memcmp(mKey,key,KEY_SIZE);
    crypto::SecureClearArray(key,KEY_SIZE);
    if(0==compResult){

        mHeaderV1.data.sign=CONTAINER_SIGN;
        mHeaderV1.data.headerSize=HEADERV1_SIZE;
        mHeaderV1.data.containerVersion=CONTAINER_VERSION_V1;
        RandomNumber::Generate(mHeaderV1.data.salt,SALT_SIZE);
        mHeaderV1.data.argon2IterationCost=argon2ItCost;
        mHeaderV1.data.argon2MemoryCost=argon2MemCost;
        mHeaderV1.data.argon2Lanes=argon2Lanes;
        mHeaderV1.data.headerCrc=0;
        CreatePasswordHash(newPassword,newPasswordSize,mHeaderV1.data.salt,mKey);

        return Save();
    }else{
        return ePasswordWronng;
    }
}


///
/// \brief      Creates hash a argon2 hash of the password
///             and salt
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void ContainerEncrypted::CreatePasswordHash(const char* password, uint32_t passwordSize, const char* salt,char* hash) const
{
    Argon2_Context context;
    context.out=(uint8_t*)hash;
    context.outlen=KEY_SIZE;
    context.pwd=(uint8_t*)password;
    context.pwdlen=passwordSize;
    context.salt=(uint8_t*)salt;
    context.saltlen=SALT_SIZE;
    context.secret=NULL;
    context.secretlen=0;
    context.ad=NULL;
    context.adlen=0;
    context.t_cost=mHeaderV1.data.argon2IterationCost;
    context.m_cost=mHeaderV1.data.argon2MemoryCost;
    context.lanes=mHeaderV1.data.argon2Lanes;
    context.threads=1;
    context.version=19;
    context.allocate_cbk=NULL;
    context.free_cbk=NULL;

    ///////////////////////////////////////////////////////////////////////////////
    /// Compute Argon2 hash
    int error = argon2i_ctx(&context);
    if(0!=error){
        throw eArgon2Error;
    }
}

///
/// \brief       Returns the actual argon2 settings
/// \author      Joachim Danmayr
/// \date        2016-10-08
///
/// \param[out]  itCost  Iteration cost fir password hashing
/// \param[out]  memCost Memory cost for password hashing [kB]
/// \param[out]  lanes   Number of lanes for password hashing
///
void ContainerEncrypted::GetArgon2Setting(uint32_t& itCost, uint32_t& memCost, uint32_t& lanes)
{
    itCost  = mHeaderV1.data.argon2IterationCost;
    memCost = mHeaderV1.data.argon2MemoryCost;
    lanes   = mHeaderV1.data.argon2Lanes;
}

///
/// \brief      Creates a new session key for
///             in RAM encryption
///             and salt
/// \author     Joachim Danmayr
/// \date       2016-10-08
///
void ContainerEncrypted::CreateSessionKey()
{
   RandomNumber::Generate(mSessionKey,sizeof(mSessionKey));
}
