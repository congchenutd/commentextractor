#ifndef COMMENTEDIT_H
#define COMMENTEDIT_H

#include <QTextEdit>

class CommentEdit : public QTextEdit
{
public:
    CommentEdit(QWidget* parent = 0);
    void setFile(const QString& filePath, int lineNum);   // load the file and scroll to the line
};

#endif // COMMENTEDIT_H
