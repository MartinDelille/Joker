HEADERS += \
#    ../../libs/PhCommonUI/PhLeap.h \
	../../libs/PhCommonUI/PhMediaPanel.h \
	../../libs/PhCommonUI/PhTimecodeEdit.h \
    ../../libs/PhCommonUI/PhTimeCodeDialog.h \
<<<<<<< HEAD
    ../../libs/PhCommonUI/PhMediaPanelDialog.h
=======
    ../../libs/PhCommonUI/PhFontDialog.h \
    ../../libs/PhCommonUI/PhFloatingMediaPanel.h
>>>>>>> origin/master

SOURCES += \
#   ../../libs/PhCommonUI/PhLeap.cpp \
	../../libs/PhCommonUI/PhMediaPanel.cpp \
	../../libs/PhCommonUI/PhTimecodeEdit.cpp \
    ../../libs/PhCommonUI/PhTimeCodeDialog.cpp \
<<<<<<< HEAD
    ../../libs/PhCommonUI/PhMediaPanelDialog.cpp
=======
    ../../libs/PhCommonUI/PhFontDialog.cpp \
    ../../libs/PhCommonUI/PhFloatingMediaPanel.cpp
>>>>>>> origin/master


FORMS += ../../libs/PhCommonUI/PhMediaPanel.ui \
    ../../libs/PhCommonUI/PhTimeCodeDialog.ui \
<<<<<<< HEAD
    ../../libs/PhCommonUI/PhMediaPanelDialog.ui
=======
    ../../libs/PhCommonUI/PhFontDialog.ui
>>>>>>> origin/master

#INCLUDEPATH += $$(LEAP_SDK)/include

# Windows specific
win32 {
}

# Ubuntu specific
linux {
#LIBS += -L$$(LEAP_SDK)/lib/x64 -lLeap
}


# MacOS specific
mac {
#LIBS += -L$$(LEAP_SDK)/lib -lLeap
}

#	leap.commands += cp -r $$(LEAP_SDK)/lib/libLeap.dylib $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
