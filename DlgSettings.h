#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include "ui_DlgSettings.h"

class DlgSettings : public QDialog
{
    Q_OBJECT
    
public:
    DlgSettings(QWidget* parent = 0);
    void accept();

    QString getNameFilterString() const;
    QString getCommentFilter()    const;
    QString getTagFilter()        const;
    int     getRandomPickSize()   const;
    int     getRemoveSmallSize()  const;
    bool    useRegEx()            const;
    QFont   getUIFont()           const;
    QFont   getEditorFont()       const;

    void setUIFont    (const QFont& font);
    void setEditorFont(const QFont& font);

private slots:
    void onUIFont();
    void onEditorFont();

private:
    void loadSettings();
    void saveSettings();

    QString getExportModularity() const;
    void setExportModularity(const QString& modularity);

private:
    Ui::DlgSettings ui;
};

#endif // DLGSETTINGS_H
