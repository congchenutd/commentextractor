#include "MyStatusBar.h"
#include <QApplication>
#include <QClipboard>
#include <QDir>

MyStatusBar::MyStatusBar(QWidget* parent) :
    QStatusBar(parent) {}

void MyStatusBar::mouseDoubleClickEvent(QMouseEvent*) {
    QApplication::clipboard()->setText(QDir::toNativeSeparators(currentMessage()));
}
