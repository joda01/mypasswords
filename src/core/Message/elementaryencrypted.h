///
/// \file       elementary.h
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      Base type for an element in the container
///

#ifndef ELEMENTARY_ENCRYPTEF_H
#define ELEMENTARY_ENCRYPTEF_H

#include "type.h"

namespace msg {

///
/// \class      ElementaryEncrypted
/// \author     Joachim Danmayr
/// \date       2016-09-18
///
/// \brief      An elementary data entry.
///             Holds data in a char array
///
class ElementaryEncrypted : public Type
{
public:
    //////////////////////////////////////////////////////////////////////
    /// Constants
    static const int32_t MAX_ENCRYPTED_DATA_SIZE = 512;   ///< Maximum allowd bytes of an encrypted message

    //////////////////////////////////////////////////////////////////////
    /// Methods
    ElementaryEncrypted(uint32_t id, const char* sessionKey);
    ElementaryEncrypted& operator=(const ElementaryEncrypted&);
    ~ElementaryEncrypted();
    virtual void Serialize(std::ostream& stream) override;
    virtual void Deserialize(std::istream& stream) override;
    virtual uint32_t GetSize() override;
    virtual void Clear(void) override;
    virtual char* GetData(uint32_t& size){size=mDataSize;Decrypt();return mData;}
    virtual void SetData(const char *data, uint32_t size);
    void Encrypt();
private:
    //////////////////////////////////////////////////////////////////////
    /// Enums
    enum EncryptState_t{eEncrypted, eDecrypted};
    //////////////////////////////////////////////////////////////////////
    /// Methods
    void Decrypt();
    //////////////////////////////////////////////////////////////////////
    /// Members
    uint32_t mDataSize;                 ///< Size of the stored data
    char* mData;                        ///< Stored data
    const char* mSessionKey;            ///< Pointer to the session key. This key is used for in RAM encryption
    EncryptState_t mState;              ///< State to store if the data er encrypted or decrypted
};
}
#endif // ELEMENTARY_H
