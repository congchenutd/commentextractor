#include "CodeEdit.h"
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QTextBlock>

CodeEdit::CodeEdit(QWidget* parent) : QTextEdit(parent) {}

void CodeEdit::loadFile(const QString& filePath, int lineNum)
{
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
        return;

    // read file
    QTextStream os(&file);
    setText(os.readAll());

    // scroll to bottom so that the line will be shown at the top of the page
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());

    // find the block containing lineNum, and move cursor to the block
    QTextBlock block = document()->findBlockByLineNumber(lineNum - 1);
    QTextCursor cursor(block);
    setTextCursor(cursor);
    ensureCursorVisible();

    // highlight the line
    QTextEdit::ExtraSelection highlight;
    highlight.cursor = cursor;
    highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
    highlight.format.setBackground(Qt::yellow);
    setExtraSelections(QList<QTextEdit::ExtraSelection>() << highlight);
}
