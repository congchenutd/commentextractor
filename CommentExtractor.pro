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
    TagFilter.cpp \
    Highlighter.cpp \
    MainWindow.cpp \
    DlgSettings.cpp \
    TextBlock.cpp \
    Settings.cpp \
    CommentModel.cpp \
    CodeEdit.cpp \
    TagInstanceModel.cpp \
    TagKeywordModel.cpp \
    MyStatusBar.cpp

HEADERS  += \
    Extractor.h \
    Counter.h \
    Writer.h \
    Runnable.h \
    TextBlock.h \
    ProgressDisplay.h \
    FileLog.h \
    TagFilter.h \
    Highlighter.h \
    MainWindow.h \
    DlgSettings.h \
    Settings.h \
    CommentModel.h \
    CodeEdit.h \
    TagInstanceModel.h \
    TagKeywordModel.h \
    MyStatusBar.h

FORMS    += \
    MainWindow.ui \
    DlgSettings.ui

RESOURCES += \
    Resource.qrc
