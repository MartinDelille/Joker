TEMPLATE = app
TARGET = VideoTest

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhVideo/PhVideo.pri)
include($${JOKER_ROOT}/libs/PhCommonUI/PhCommonUI.pri)

HEADERS += MainView.h \
    PhVideoSynchronizer.h
	VideoPropertiesDialog.h \
	peVideoPropertiesDialog

SOURCES += \
    main.cpp \
    MainView.cpp \
    PhVideoSynchronizer.cpp
	VideoPropertiesDialog.cpp \
	peVideoPropertiesDialog

FORMS += \
    MainView.ui \
    VideoPropertiesDialog.ui

