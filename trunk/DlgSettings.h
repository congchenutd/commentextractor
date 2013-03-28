#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include "ui_DlgSettings.h"
#include <QSettings>

class QFont;

class Settings : public QSettings
{
public:
    Settings(const QString& fileName = "settings.ini");

    QString     getNameFilterString() const;
    QStringList getNameFilter()       const;
    QString     getContentFilter()    const;
    int         getRandomPickSize()   const;
    int         getRemoveSmallSize()  const;
    bool        useRegEx()            const;
    QFont       getUIFont()           const;
    QFont       getEditorFont()       const;
    QByteArray  getSplitterState()    const;
    QString     getLastPath()         const;
    bool        getExportByPackage()  const;

    void setNameFilterString(const QString& filter);
    void setTagFilter       (const QString& filter);
    void setRandomPickSize  (int size);
    void setRemoveSmallSize (int size);
    void setUseRegEx        (bool useRegEx);
    void setUIFont          (const QFont& font);
    void setEditorFont      (const QFont& font);
    void setSplitterState   (const QByteArray& state);
    void setLastPath        (const QString& path);
    void setExportByPackage (bool byPackage);
};

class DlgSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSettings(QWidget* parent = 0);
    void accept();

    QString getNameFilterString() const;
    QString getContentFilter()    const;
    int     getRandomPickSize()   const;
    int     getRemoveSmallSize()  const;
    bool    useRegEx()            const;
    QFont   getUIFont()           const;
    QFont   getEditorFont()       const;
    bool    getExportByPackage()  const;

    void setUIFont    (const QFont& font);
    void setEditorFont(const QFont& font);

private slots:
    void onUIFont();
    void onEditorFont();

private:
    void loadSettings();
    void saveSettings();

private:
    Ui::DlgSettings ui;
};

#endif // DLGSETTINGS_H
