#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "Runnable.h"
#include "Settings.h"

class QProgressBar;
class QDirIterator;
class QLabel;
class Extractor;
class TagFilter;
class TagCountModel;
class TagInstanceModel;
class TagCounter;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget* parent = 0);

protected:
    void closeEvent(QCloseEvent*);

private slots:
    void onExtract();
    void onPick();
    void onLoad();
    void onSave();
    void onTagClicked(const QModelIndex& idx);
    void onTagInstanceClicked(const QModelIndex& idx);
    void onDeleteTag();
    void onExport();
    void onSettings();

private:
    void loadSettings();
    void saveSettings();

    QStringList getNameFilter()      const { return _settings.getNameFilter(); }
    QString     getContentFilter()   const { return _settings.getContentFilter(); }
    int         getRandomPickSize()  const { return _settings.getRandomPickSize(); }
    int         getRemoveSmallSize() const { return _settings.getRemoveSmallSize(); }
    bool        useRegEx()           const { return _settings.useRegEx(); }

    QDirIterator* createIterator() const;
    void iterate(QDirIterator* iterator, Actors& actors);   // iterate files and apply actions
    int countFiles();

    void setCurrentInstanceModel(TagInstanceModel* model);
    void setProjectPath(const QString& projectPath);

private:
    Ui::MainWindow ui;

    Settings         _settings;
    TagCountModel*    _modelCount;
    TagInstanceModel* _modelInstances;
    TagCounter*       _tagCounter;
    
    QLabel* _labelFileCount;
    QLabel* _labelLineCount;
    QLabel* _labelTagCount;
    QProgressBar* _progressBar;
};

#endif // MAINWINDOW_H
