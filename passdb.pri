SOURCES += \
    src/core/Crypto/hashing/byte_order.cpp \
    src/core/Crypto/RandomNumber.cpp \
    src/core/Crypto/encryption/Rijndael.cpp \
    src/core/NativeFunctions/NativeFunctions.cpp \
    src/core/Crypto/Secure.cpp \
    src/main.cpp \
    src/core/Container/Container.cpp \
    src/core/Message/elementary.cpp \
    src/core/Message/nested.cpp \
    src/guiQt/DialogSettings.cpp \
    src/guiQt/tablemodel.cpp \
    src/guiQt/windowmain.cpp \
    src/guiQt/DialogPasswordGenerator.cpp \
    src/core/Crypto/PasswordGenerator.cpp \
    src/guiQt/modelpasswordentry.cpp \
    src/guiQt/modelpasswordelements.cpp \
    src/guiQt/dialogpasswordenty.cpp \
    src/guiQt/dialoggroup.cpp \
    src/core/Crypto/uuidgenerator.cpp \
    src/guiQt/treemodel.cpp \
    src/core/Container/containerencrypted.cpp \
    src/guiQt/dialogabout.cpp \
    src/core/Crypto/hashing/sha3.cpp \
    src/guiQt/dialogopencontainer.cpp \
    src/core/Message/elementaryencrypted.cpp \
    $$PWD/src/core/Crypto/hashing/argon/Blake2/blake2b.c \
    $$PWD/src/core/Crypto/hashing/argon/argon2.c \
    $$PWD/src/core/Crypto/hashing/argon/core.c \
    $$PWD/src/core/Crypto/hashing/argon/encoding.c \
    $$PWD/src/core/Crypto/hashing/argon/thread.c \
    $$PWD/src/core/Crypto/hashing/argon/ref.c \
    src/guiQt/wizardnewcontainer.cpp \
    $$PWD/src/core/Crypto/hashing/argon.cpp \
    $$PWD/src/guiQt/passwordcontainerthread.cpp \
    src/guiQt/dialogwait.cpp \
    $$PWD/src/guiQt/qglobalshortcut/qglobalshortcut.cc \
    $$PWD/src/guiQt/qglobalshortcut/qglobalshortcut_win.cc \
    $$PWD/src/guiQt/qglobalshortcut/qglobalshortcut_x11.cc \
    $$PWD/src/guiQt/autotype.cpp \
    $$PWD/src/guiQt/configuration.cpp \
    $$PWD/src/web/websocketserver.cpp \
    src/guiQt/dialogcomment.cpp \
    $$PWD/src/plugin/pluginwebsocket.cpp \
    $$PWD/src/web/smtp.cpp \
    src/guiQt/dialogimport.cpp \
    $$PWD/src/core/Crypto/zxcvbn/zxcvbn.c


HEADERS  += \
    src/core/Crypto/hashing/byte_order.h \
    src/core/Crypto/hashing/ustd.h \
    src/core/Crypto/RandomNumber.h \
    src/core/Crypto/encryption/Rijndael.h \
    src/core/NativeFunctions/NativeFunctions.h \
    src/core/Crypto/Secure.h \
    src/core/Container/Container.h \
    src/core/Message/type.h \
    src/core/Message/elementary.h \
    src/core/Message/nested.h \
    src/core/Message/serializable.h \
    src/core/Message/repeated.h \
    src/guiQt/DialogSettings.h \
    src/guiQt/tablemodel.h \
    src/guiQt/windowmain.h \
    src/guiQt/DialogPasswordGenerator.h \
    src/core/Crypto/PasswordGenerator.h \
    src/guiQt/modelpasswordentry.h \
    src/guiQt/modelpasswordelements.h \
    src/guiQt/dialogpasswordenty.h \
    src/guiQt/dialoggroup.h \
    src/core/Crypto/uuidgenerator.h \
    src/guiQt/treemodel.h \
    src/core/Container/containerencrypted.h \
    src/guiQt/dialogabout.h \
    src/core/Crypto/hashing/sha3.h \
    src/guiQt/dialogopencontainer.h \
    src/core/Message/elementaryencrypted.h \
    $$PWD/src/core/Crypto/hashing/argon/Blake2/blake2-impl.h \
    $$PWD/src/core/Crypto/hashing/argon/Blake2/blake2.h \
    $$PWD/src/core/Crypto/hashing/argon/Blake2/blamka-round-ref.h \
    $$PWD/src/core/Crypto/hashing/argon/argon2.h \
    $$PWD/src/core/Crypto/hashing/argon/core.h \
    $$PWD/src/core/Crypto/hashing/argon/encoding.h \
    $$PWD/src/core/Crypto/hashing/argon/thread.h \
    $$PWD/src/core/Crypto/hashing/argon/ref.h \
    $$PWD/src/version.h \
    $$PWD/src/core/Crypto/hashing/hashing.h \
    $$PWD/src/core/Crypto/encryption/encryption.h \
    src/guiQt/wizardnewcontainer.h \
    $$PWD/src/core/Crypto/hashing/argon.h \
    $$PWD/src/guiQt/passwordcontainerthread.h \
    src/guiQt/dialogwait.h \
    $$PWD/src/core/NativeFunctions/rlutil.h \
    $$PWD/src/guiQt/qglobalshortcut/qglobalshortcut.h \
    $$PWD/src/guiQt/autotype.h \
    $$PWD/src/guiQt/configuration.h \
    $$PWD/src/web/websocketserver.h \
    src/guiQt/dialogcomment.h \
    $$PWD/src/plugin/pluginwebsocket.h \
    $$PWD/src/web/smtp.h \
    src/guiQt/dialogimport.h \
    $$PWD/src/core/Crypto/zxcvbn/zxcvbn.h


FORMS += \
    src/guiQt/DialogPasswordGenerator.ui \
    src/guiQt/DialogSettings.ui \
    src/guiQt/windowmain.ui \
    src/guiQt/dialogpasswordenty.ui \
    src/guiQt/dialoggroup.ui \
    src/guiQt/dialogabout.ui \
    src/guiQt/dialogopencontainer.ui \
    src/guiQt/wizardnewcontainer.ui \
    src/guiQt/dialogwait.ui \
    src/guiQt/dialogcomment.ui \
    src/guiQt/dialogimport.ui

RESOURCES += \
    resources/resource.qrc


TRANSLATIONS = languages/passwordstr_en-US.ts \
               languages/passwordstr_es-ES.ts \
               languages/passwordstr_fr-FR.ts \
               languages/passwordstr_de-DE.ts \
               languages/passwordstr_it-IT.ts \
               languages/passwordstr_sv-SV.ts \
               languages/passwordstr_jp-JP.ts

win32 {
RC_FILE = $$PWD/passdb.rc

DISTFILES += \
    $$PWD/passdb.rc
}
unix{
    DEFINES += LINUX
    QT += x11extras
    LIBS += -lX11 -lxcb
}
