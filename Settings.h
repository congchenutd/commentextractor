#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
    Settings(const QString& fileName = "settings.ini");

    QString     getNameFilterString() const;
    QStringList getNameFilter()       const;
    QString     getCommentFilter()    const;
    QString     getTagFilter()        const;
    int         getRandomPickSize()   const;
    int         getRemoveSmallSize()  const;
    bool        useRegEx()            const;
    QFont       getUIFont()           const;
    QFont       getEditorFont()       const;
    QString     getLastPath()         const;
    QString     getExportModularity() const;
    QString     getProjectPath()      const;

    void setNameFilterString(const QString& filter);
    void setCommentFilter   (const QString& filter);
    void setTagFilter       (const QString& filter);
    void setRandomPickSize  (int size);
    void setRemoveSmallSize (int size);
    void setUseRegEx        (bool useRegEx);
    void setUIFont          (const QFont& font);
    void setEditorFont      (const QFont& font);
    void setSplitterState   (const QByteArray& state);
    void setLastPath        (const QString& path);
    void setExportModularity(const QString& modularity);
    void setProjectPath     (const QString& path);
};

#endif // SETTINGS_H
