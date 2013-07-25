#ifndef COMMENTEDIT_H
#define COMMENTEDIT_H

#include <QTextEdit>

// An textedit that can scroll to and highlight specified line
class CodeEdit : public QTextEdit
{
public:
    CodeEdit(QWidget* parent = 0);

    // load the file and scroll to the line
    void loadFile(const QString& filePath, int lineNum);
};

#endif // COMMENTEDIT_H
