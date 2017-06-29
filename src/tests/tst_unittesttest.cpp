#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <ctime>

#include "../core/Container/containerencrypted.h"
#include "../core/Crypto/argon/argon2.h"


class UnittestTest : public QObject
{
    Q_OBJECT

public:
    UnittestTest();

private Q_SLOTS:
    void testCase1_data();
    void testGroupEntry();
    void testArgon2();
    void testArgon2RuntimeCheck();
    void testSha3();
};

UnittestTest::UnittestTest()
{
}

void UnittestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}



#define PASSWORD_CONTAINER  "unitTest.jpx"
#define PASSWORD "123"
#define PASSWORD_SIZE 3
#define GROUP_ENTRY 10

void UnittestTest::testGroupEntry()
{
    return;
    for(uint32_t grp=0;grp<GROUP_ENTRY;grp++){

        //////////////////////////////////////////////////////////////////////////////////
        /// Create new db file
        ///
        QFile file(PASSWORD_CONTAINER);
        file.remove();
        file.open(QIODevice::ReadWrite);
        file.close();

        ContainerEncrypted* mPasswordContainer = new ContainerEncrypted();

        mPasswordContainer->Open(PASSWORD_CONTAINER,PASSWORD,PASSWORD_SIZE);
        mPasswordContainer->Save();
        mPasswordContainer->Close();

        //////////////////////////////////////////////////////////////////////////////////
        /// Add groups
        ///
        mPasswordContainer->Open(PASSWORD_CONTAINER,PASSWORD,PASSWORD_SIZE);

        ContainerEncrypted::GroupEntry* group;
        for(uint32_t n=0;n<grp;n++){
            group = new ContainerEncrypted::GroupEntry();
            QString name = QString("Gruppe%1").arg(n);

            group->SetName(name.toUtf8().data(),name.toUtf8().size());
            group->SetParentId("",0);
            mPasswordContainer->AddGroup(group);
        }

        mPasswordContainer->Save();
        mPasswordContainer->Close();


        for(int u=0;u<2;u++){
            //////////////////////////////////////////////////////////////////////////////////
            /// Load groups
            ///
            mPasswordContainer->Open(PASSWORD_CONTAINER,PASSWORD,PASSWORD_SIZE);
            uint32_t elementNr = mPasswordContainer->GetNumberOfGroupEntry();

            QVERIFY(elementNr==grp);
            QString uuidStringOld = "";
            for(uint32_t n=0;n<elementNr;n++){
                uint32_t size;char* name = mPasswordContainer->GetGroup(n)->GetName(size);
                QString nameToComp = QString::fromUtf8(name,size);
                QString should = QString("Gruppe%1").arg(n);
                QCOMPARE(nameToComp,should);

                // UUID must not be equal
                uint32_t sizeuuid;const char* uuid = mPasswordContainer->GetGroup(n)->GetGroupId(sizeuuid);
                QString uuidString = QString::fromLocal8Bit(uuid,sizeuuid);
                QVERIFY(uuidStringOld!=uuidString);
                uuidStringOld = uuidString;
            }
            mPasswordContainer->Close();
        }

        delete mPasswordContainer;
    }

    //QFETCH(QString, data);
    //QVERIFY2(true, "Failure");
}


///
/// \brief      Argon2 test vectors
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
void UnittestTest::testArgon2()
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

    ///////////////////////////////////////////////////////////////////////////////
    /// Compute Argon2d hash
    int error = argon2d_ctx(&context);
    QVERIFY(error==0);
    unsigned char expectedOutputD[] = {0x51,0x2b,0x39,0x1b,0x6f,0x11,0x62,0x97,0x53,0x71,0xd3,0x09,0x19,0x73,0x42,0x94,0xf8,0x68,0xe3,0xbe,0x39,0x84,0xf3,0xc1,0xa1,0x3a,0x4d,0xb9,0xfa,0xbe,0x4a,0xcb};
    int hashAndExpectedHashEqual = memcmp(expectedOutputD,hash,hashlen);
    QVERIFY(hashAndExpectedHashEqual==0);

    ///////////////////////////////////////////////////////////////////////////////
    /// Compute Argon2i hash
    memset(secret, 3, 256);
    context.secretlen=secretlen;
    error = argon2i_ctx(&context);
    QVERIFY(error==0);
    unsigned char expectedOutputI[] = {0xc8,0x14,0xd9,0xd1,0xdc,0x7f,0x37,0xaa,0x13,0xf0,0xd7,0x7f,0x24,0x94,0xbd,0xa1,0xc8,0xde,0x6b,0x01,0x6d,0xd3,0x88,0xd2,0x99,0x52,0xa4,0xc4,0x67,0x2b,0x6c,0xe8};
    hashAndExpectedHashEqual = memcmp(expectedOutputI,hash,hashlen);
    QVERIFY(hashAndExpectedHashEqual==0);

    /*for (unsigned i = 0; i < hashlen; ++i){
        printf("%2.2x", ((unsigned char*)hash)[i]);
    }*/
    std::cout << std::endl;

}

///
/// \brief      Argon2 test vectors
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
void UnittestTest::testArgon2RuntimeCheck()
{

    unsigned char password[256];
    memset(password, 1, 256);
    unsigned pwdlen = 10;

    unsigned char salt[256];
    memset(salt, 2, 256);
    unsigned saltlen = 32;

    unsigned char secret[256];
    memset(secret, 3, 256);
    unsigned secretlen = 0;

    unsigned char ad[256];
    memset(ad, 4, 256);
    unsigned adlen = 0;

    unsigned char hash[128];
    uint16_t hashlen = 32;

    unsigned t_cost = 50;
    unsigned m_cost = 1024;
    unsigned lanes = 16;


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


    ///////////////////////////////////////////////////////////////////////////////
    /// Compute Argon2i hash
    std::cout << "Argon2 Start" << std::endl;
    clock_t begin = clock();
    int error = argon2i_ctx(&context);
    QVERIFY(error==0);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Argon2 end: " << elapsed_secs  << std::endl;

}






///
/// \brief      SHA3 test vectors
/// \author     Joachim Danmayr
/// \date       2016-10-16
///
void UnittestTest::testSha3()
{

}


QTEST_MAIN(UnittestTest)

#include "tst_unittesttest.moc"
