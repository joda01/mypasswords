///
/// \file       Rijndael.h
/// \author     Joachim Danmayr
/// \date       2016-09-21
///
/// \brief      Rijndael (pronounced Reindaal) is a block cipher,
///             designed by Joan Daemen and Vincent Rijmen as a candidate
///             algorithm for the AES.
///             The cipher has a variable block length and key length. The
///             authors currently specify how to use keys with a length
///             of 128, 192, or 256 bits to encrypt blocks with al length of
///             128, 192 or 256 bits (all nine combinations of key length and
///             block length are possible). Both block length and key length can
///             be extended very easily to multiples of 32 bits.
///             Rijndael can be implemented very efficiently on a wide range of
///             processors and in hardware. This implementation is based on the
///             Java Implementation used with the Cryptix toolkit found at:
///             http://www.esat.kuleuven.ac.be/~rijmen/rijndael/rijndael.zip
///             Java code authors: Raif S. Naffah, Paulo S. L. M. Barreto
///             This Implementation was tested against KAT test published by the
///             authors of the method and the results were identical.
///

#ifndef RIJNDAEL_H_
#define RIJNDAEL_H_

#include <exception>
#include <cstring>
#include "encryption.h"

using namespace std;


class CRijndael : public Encryption
{
public:

    enum { DEFAULT_BLOCK_SIZE=16 };
    enum { MAX_BLOCK_SIZE=32, MAX_ROUNDS=14, MAX_KC=8, MAX_BC=8 };
    enum EncryptMode_t{ ECB=0, CBC=1, CFB=2 };

    CRijndael(char const* key, char const* chain, int keylength, int blockSize,EncryptMode_t iMode);
    ~CRijndael();
    virtual void Encrypt(char const* in, char* result, uint32_t n) override;
    virtual void Decrypt(char const* in, char* result, uint32_t n) override;

private:
    void MakeKey(char const* key, char const* chain, int keylength, int blockSize);
    void EncryptBlock(char const* in, char* result);
    void DecryptBlock(char const* in, char* result);

    EncryptMode_t miMode;

    static char const* sm_chain0;   ///< Null chain
    static const int sm_alog[256];
    static const int sm_log[256];
    static const char sm_S[256];
    static const char sm_Si[256];
    static const int sm_T1[256];
    static const int sm_T2[256];
    static const int sm_T3[256];
    static const int sm_T4[256];
    static const int sm_T5[256];
    static const int sm_T6[256];
    static const int sm_T7[256];
    static const int sm_T8[256];
    static const int sm_U1[256];
    static const int sm_U2[256];
    static const int sm_U3[256];
    static const int sm_U4[256];
    static const char sm_rcon[30];
    static const int sm_shifts[3][4][2];
    static char const* sm_szErrorMsg1;      ///< Error Messages
    static char const* sm_szErrorMsg2;      ///< Error Messages

    void DefEncryptBlock(char const* in, char* result);
    void DefDecryptBlock(char const* in, char* result);

    //Get Key Length
    int GetKeyLength(){
        if(false==m_bKeyInit){
            //throw exception(sm_szErrorMsg1);
            return 0;
        }
        return m_keylength;
    }

    //Block Size
    int	GetBlockSize(){
        if(false==m_bKeyInit){
            //throw exception(sm_szErrorMsg1);
            return 0;
        }
        return m_blockSize;
    }

    //Number of Rounds
    int GetRounds(){
        if(false==m_bKeyInit){
            //throw exception(sm_szErrorMsg1);
            return 0;
        }
        return m_iROUNDS;
    }

    void ResetChain(){memcpy(m_chain, m_chain0, m_blockSize);}


    //Auxiliary Functions
    //Multiply two elements of GF(2^m)
    static int Mul(int a, int b){return (a != 0 && b != 0) ? sm_alog[(sm_log[a & 0xFF] + sm_log[b & 0xFF]) % 255] : 0;}

    //Convenience method used in generating Transposition Boxes
    static int Mul4(int a, char b[]){
        if(a == 0){
            return 0;
        }
        a = sm_log[a & 0xFF];
        int a0 = (b[0] != 0) ? sm_alog[(a + sm_log[b[0] & 0xFF]) % 255] & 0xFF : 0;
        int a1 = (b[1] != 0) ? sm_alog[(a + sm_log[b[1] & 0xFF]) % 255] & 0xFF : 0;
        int a2 = (b[2] != 0) ? sm_alog[(a + sm_log[b[2] & 0xFF]) % 255] & 0xFF : 0;
        int a3 = (b[3] != 0) ? sm_alog[(a + sm_log[b[3] & 0xFF]) % 255] & 0xFF : 0;
        return a0 << 24 | a1 << 16 | a2 << 8 | a3;
    }


    void Xor(char* buff, char const* chain){
        if(false==m_bKeyInit){
            //throw exception(sm_szErrorMsg1);
            return;
        }
        for(int i=0; i<m_blockSize; i++){
            *(buff++) ^= *(chain++);
        }
    }

    bool m_bKeyInit;                        ///< Key Initialization Flag
    int  m_Ke[MAX_ROUNDS+1][MAX_BC];        ///< Encryption (m_Ke) round key
    int  m_Kd[MAX_ROUNDS+1][MAX_BC];        ///< Decryption (m_Kd) round key
    int  m_keylength;                       ///< Key Length
    int	 m_blockSize;                       ///< Block Size
    int  m_iROUNDS;                         ///< Number of Rounds
    char m_chain0[MAX_BLOCK_SIZE];          ///< Chain Block
    char m_chain[MAX_BLOCK_SIZE];
    int  tk[MAX_KC];                        ///< Auxiliary private use buffers
    int  a[MAX_BC];
    int  t[MAX_BC];
};

#endif // __RIJNDAEL_H__
