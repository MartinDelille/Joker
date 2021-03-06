#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

TARGET = VideoTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhVideo/PhVideo.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)

HEADERS += \
	$$PWD/VideoTestWindow.h \
	$$PWD/VideoTestSettings.h \
    $$PWD/PropertyDialog.h

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/VideoTestWindow.cpp \
    $$PWD/PropertyDialog.cpp

FORMS += \
	$$PWD/VideoTestWindow.ui \
    $$PWD/PropertyDialog.ui

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)

