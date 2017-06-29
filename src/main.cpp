#include <QApplication>
#include <iostream>
#include <sstream>
#include <QDebug>
#include <QString>
#include "core/NativeFunctions/rlutil.h"
#include <fstream>
#include <QtWebSockets/QtWebSockets>
#include "guiQt/windowmain.h"
#include "core/Container/Container.h"
#include "core/Crypto/hashing/argon/argon2.h"
#include "core/Crypto/hashing/sha3.h"
#include "core/Crypto/encryption/Rijndael.h"
#include "core/Crypto/RandomNumber.h"
#include "version.h"


static const int32_t WARNINGCODE = 1;
static const int32_t ERRORCODE   = 2;

int testArgon2();
int testAES();
int testSha3();
int testRandomGenerator();
int testIsRelease();
void printPassedOrFaild(int failed);

#ifndef UNIT_TEST

int32_t main(int32_t argc, char *argv[])
{
    QApplication::setStyle("fusion");
    QApplication a(argc, argv);
    QApplication::setStyle("fusion");
    rlutil::setColor(rlutil::GREY);
    //rlutil::setBackgroundColor(rlutil::BLACK);

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "-   PassDB "  << VER_PRODUCTVERSION_STR << ", " << VER_BUILD_DATE_STR << std::endl;
    std::cout << "-   Copyright 2015-2017 by Joachim Danmayr                    -" << std::endl;
    std::cout << "-                                                             -" << std::endl;
    std::cout << "- This application is licensed under the terms of LGPLv3.     -" << std::endl;
    std::cout << "- Please contact me if you want to use it for commercial.     -" << std::endl;
    std::cout << "-                                                             -" << std::endl;
    std::cout << "- Enjoy the use...                                            -" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "  * starting selftest..." << std::endl;
    int error = testArgon2();
    std::cout << "  * argon2 testvectors   ";printPassedOrFaild(error);
    error = testAES();
    std::cout << "  * aes    testvectors   ";printPassedOrFaild(error);
    error = testSha3();
    std::cout << "  * sha3   testvectors   ";printPassedOrFaild(error);
    error = testRandomGenerator();
    std::cout << "  * random generator     ";printPassedOrFaild(error);
    error = testIsRelease();
    std::cout << "  * is release build     ";printPassedOrFaild(error);
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "  * starting graphical user interface..." << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    Configuration settings;

    QString translationFile("passwordstr_");
    translationFile.append(settings.GetLanguage());
    QTranslator translator;
    if (translator.load(translationFile,"../../languages"))
        a.installTranslator(&translator);
    else
        std::cout << "Main:LoadLanguage: Language not found" << std::endl;


    WindowMain* main = new WindowMain(&settings);
    a.installEventFilter(main);

    main->showWindow();
    int ret = a.exec();
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "- Thank you and goodbye!                                      -" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;

    return ret;
}
#endif


///
/// \brief      Prints passed to the console if the input is 0
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
/// \param[in]  failed  0 if everything is ok 1 warning, 2 error
///
void printPassedOrFaild(int failed)
{
    if(0==failed){
        rlutil::setColor(rlutil::LIGHTGREEN);
        std::cout << "passed" << std::endl;

    }
    else if(WARNINGCODE==failed){
        rlutil::setColor(rlutil::LIGHTCYAN);
        std::cout << "warning" << std::endl;
    }
    else
    {
        rlutil::setColor(rlutil::LIGHTRED);
        std::cout << "failed" << std::endl;
    }
    rlutil::setColor(rlutil::GREY);
}


///
/// \brief      Argon2 test vectors
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
/// \return     0 if everthing was ok
///
int testArgon2()
{
    unsigned char password[256];
    memset(password, 1, 256);
    unsigned pwdlen = 32;

    unsigned char salt[256];
    memset(salt, 2, 256);
    unsigned saltlen = 16;

    unsigned char secret[256];
    memset(secret, 3, 256);
    unsigned secretlen = 8;

    unsigned char ad[256];
    memset(ad, 4, 256);
    unsigned adlen = 12;

    unsigned char hash[128];
    uint16_t hashlen = 32;

    unsigned t_cost = 3;
    unsigned m_cost = 32;
    unsigned lanes = 4;


    ///////////////////////////////////////////////////////////////////////////////
    /// Compute Argon2d hash
    Argon2_Context context;
    context.out=hash;
    context.outlen=hashlen;
    context.pwd=password;
    context.pwdlen=pwdlen;
    context.salt=salt;
    context.saltlen=saltlen;
    context.secret=secret;
    context.secretlen=secretlen;
    context.ad=ad;
    context.adlen=adlen;
    context.t_cost=t_cost;
    context.m_cost=m_cost;
    context.lanes=lanes;
    context.threads=1;
    context.version=19;
    context.allocate_cbk=NULL;
    context.free_cbk=NULL;
    int error = argon2d_ctx(&context);
    unsigned char expectedOutputD[] = {0x51,0x2b,0x39,0x1b,0x6f,0x11,0x62,0x97,0x53,0x71,0xd3,0x09,0x19,0x73,0x42,0x94,0xf8,0x68,0xe3,0xbe,0x39,0x84,0xf3,0xc1,0xa1,0x3a,0x4d,0xb9,0xfa,0xbe,0x4a,0xcb};
    error |= memcmp(expectedOutputD,hash,hashlen);

    ///////////////////////////////////////////////////////////////////////////////
    /// Compute Argon2i hash
    memset(password, 1, 256);
    memset(salt, 2, 256);
    memset(secret, 3, 256);
    memset(ad, 4, 256);
    context.out=hash;
    context.outlen=hashlen;
    context.pwd=password;
    context.pwdlen=pwdlen;
    context.salt=salt;
    context.saltlen=saltlen;
    context.secret=secret;
    context.secretlen=secretlen;
    context.ad=ad;
    context.adlen=adlen;
    context.t_cost=t_cost;
    context.m_cost=m_cost;
    context.lanes=lanes;
    context.threads=1;
    context.version=19;
    context.allocate_cbk=NULL;
    context.free_cbk=NULL;
    error = argon2i_ctx(&context);
    unsigned char expectedOutputI[] = {0xc8,0x14,0xd9,0xd1,0xdc,0x7f,0x37,0xaa,0x13,0xf0,0xd7,0x7f,0x24,0x94,0xbd,0xa1,0xc8,0xde,0x6b,0x01,0x6d,0xd3,0x88,0xd2,0x99,0x52,0xa4,0xc4,0x67,0x2b,0x6c,0xe8};
    error |= memcmp(expectedOutputI,hash,hashlen);

    /*for (unsigned i = 0; i < hashlen; ++i){
        printf("%2.2x", ((unsigned char*)hash)[i]);
    }*/
    if(0!=error){
        error = ERRORCODE;
    }

    return error;
}



///
/// \brief      AES test vectors
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
/// \return     0 if everthing was ok
///
int testAES()
{
    static const int32_t DATA_LEN = 32;
    unsigned char key[]   ={0xc2,0x86,0x69,0x6d,0x88,0x7c,0x9a,0xa0,0x61,0x1b,0xbb,0x3e,0x20,0x25,0xa4,0x5a};
    unsigned char chain[] = {0x56,0x2e,0x17,0x99,0x6d,0x09,0x3d,0x28,0xdd,0xb3,0xba,0x69,0x5a,0x2e,0x6f,0x58};
    unsigned char plain[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    unsigned char out[DATA_LEN] = {0};
    unsigned char expected[] = {0xd2,0x96,0xcd,0x94,0xc2,0xcc,0xcf,0x8a,0x3a,0x86,0x30,0x28,0xb5,0xe1,0xdc,0x0a,0x75,0x86,0x60,0x2d,0x25,0x3c,0xff,0xf9,0x1b,0x82,0x66,0xbe,0xa6,0xd6,0x1a,0xb1};

    CRijndael aes((char*)key,(char*)chain,16,16,CRijndael::CBC);
    aes.Encrypt((char*)plain,(char*)out,DATA_LEN);

    int equal= memcmp(expected,out,DATA_LEN);
    if(0!=equal){
        equal = ERRORCODE;
    }
    return equal;
}


///
/// \brief      Sha3 test vectors
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
/// \return     0 if everthing was ok
///
int testSha3()
{
    crypto::Sha3 hash;
    unsigned char dataIn[]={'a','b','c'};
    unsigned char hashOut[32]={0};
    unsigned char expected[]={0x3a,0x98,0x5d,0xa7,0x4f,0xe2,0x25,0xb2,0x04,0x5c,0x17,0x2d,0x6b,0xd3,0x90,0xbd,0x85,0x5f,0x08,0x6e,0x3e,0x9d,0x52,0x5b,0x46,0xbf,0xe2,0x45,0x11,0x43,0x15,0x32};
    hash.Generate((char*)dataIn,sizeof(dataIn),(char*)hashOut,crypto::Sha3::hash256);

    int equal= memcmp(expected,hashOut,32);
    if(0!=equal){
        equal = ERRORCODE;
    }
    return equal;
}


///
/// \brief      Testing random generator
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
/// \return     0 if everthing was ok
///
int testRandomGenerator()
{
    char number1[32]={0};
    char number2[32]={0};
    RandomNumber::Generate(number1,32);
    RandomNumber::Generate(number2,32);

    int equal= memcmp(number1,number2,32);
    if(0!=equal){
        equal = 0;
    }else{
        equal = ERRORCODE;
    }
    return equal;
}

///
/// \brief      Trst if this a release build
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
/// \return     0 if everthing was ok
///
int testIsRelease()
{
#ifdef QT_DEBUG
    return WARNINGCODE;
#else
    return 0;
#endif
}
