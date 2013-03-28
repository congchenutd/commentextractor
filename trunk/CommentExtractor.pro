QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommentExtractor
TEMPLATE = app

win32 {
	RC_FILE = Resource.rc
}
mac {
	ICON = Images/Find.icns
}

SOURCES += \
    Main.cpp \
    Extractor.cpp \
    Counter.cpp \
    Writer.cpp \
    ProgressDisplay.cpp \
    FileLog.cpp \
    TagCountModel.cpp \
    TagFilter.cpp \
    TagDetailModel.cpp \
    Highlighter.cpp \
    MainWindow.cpp \
    DlgSettings.cpp \
    TextBlock.cpp

HEADERS  += \
    Extractor.h \
    Counter.h \
    Writer.h \
    Runnable.h \
    TextBlock.h \
    ProgressDisplay.h \
    FileLog.h \
    TagCountModel.h \
    TagFilter.h \
    TagDetailModel.h \
    Highlighter.h \
    MainWindow.h \
    DlgSettings.h

FORMS    += \
    MainWindow.ui \
    DlgSettings.ui

RESOURCES += \
    Resource.qrc
