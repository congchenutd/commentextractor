#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <QString>

// A block in a text file, including its content, file path, and line number
class TextBlock
{
public:
    TextBlock(const QString& content = QString(), const QString& filePath = QString(), int lineNumber = -1);

    QString getContent()     const { return _content;    }
    QString getFilePath()    const { return _filePath;   }
    int     getLineNumber()  const { return _lineNumber; }

    // NOTE: for java only
    QString getPackageName() const;
    QString getClassName()   const;         // FIXME: only for public class
    QString getCompleteClassName() const;   // package.class

    void setContent   (const QString& content)  { _content    = content;    }
    void setFilePath  (const QString& filePath) { _filePath   = filePath;   }
    void setLineNumber(int lineNumber)          { _lineNumber = lineNumber; }

private:
    QString _content;
    QString _filePath;
    int     _lineNumber;
};

#endif // TEXTBLOCK_H
