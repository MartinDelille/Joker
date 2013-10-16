QT		+= opengl

HEADERS += \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicText.h \
    ../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhFont.h \
    ../../libs/PhGraphic/PhGraphicObject.h \
    ../../libs/PhGraphic/PhGraphicRect.h \
    ../../libs/PhGraphic/PhGraphicSolidRect.h \
    ../../libs/PhGraphic/PhGraphicLoop.h

SOURCES += \
	../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
    ../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhFont.cpp \
    ../../libs/PhGraphic/PhGraphicObject.cpp \
    ../../libs/PhGraphic/PhGraphicRect.cpp \
    ../../libs/PhGraphic/PhGraphicSolidRect.cpp \
    ../../libs/PhGraphic/PhGraphicLoop.cpp

# Windows specific
win32 {
#TODO
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}

# Ubuntu specific
linux {
	INCLUDEPATH +=  /usr/include/GL \
			   /usr/include/SDL

	DEPENDPATH +=   /usr/include/GL \
			   /usr/include/SDL

	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}


# MacOS specific
mac {
	QMAKE_CXXFLAGS += -F/Library/Frameworks

	LIBS += -F/Library/Frameworks
	LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_ttf

#	copylibs.commands += cp -r /Library/Frameworks/SDL* $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/;
}

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += rm -rf $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += mkdir $${TARGET}.app/Contents/Frameworks;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL2.framework $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL2_image.framework $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL2_ttf.framework $${TARGET}.app/Contents/Frameworks/;

		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL  @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_image.framework/Versions/A/SDL_image  @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL_image $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_ttf.framework/Versions/A/SDL_ttf @executable_path/../Frameworks/SDL2_ttf.framework/Versions/A/SDL_ttf $${TARGET}.app/Contents/MacOS/$${TARGET};
	}

}
