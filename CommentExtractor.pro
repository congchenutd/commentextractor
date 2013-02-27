QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommentExtractor
TEMPLATE = app


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
    MainWindow.cpp

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
    MainWindow.h

FORMS    += \
    MainWindow.ui

RESOURCES += \
    Resource.qrc
