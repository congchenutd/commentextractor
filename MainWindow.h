#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "Runnable.h"

class QProgressBar;
class QDirIterator;
class Extractor;
class TagFilter;
class TagCountModel;
class TagDetailModel;
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
    void onOpen();
    void onSave();
    void onTagClicked(const QModelIndex& idx);
    void onDeleteTag();
    void onTagDetailClicked(const QModelIndex& idx);

private:
    void loadSettings();
    void saveSettings();

    QStringList getNameFilter()      const { return ui.leNameFilter->text().split(";"); }
    QString     getTagFilter()       const { return ui.leTagFilter->text(); }
    int         getRandomPickSize()  const { return ui.sbRandomPickSize->value(); }
    int         getRemoveSmallSize() const { return ui.sbRemoveSmallSize->value(); }
    bool        useRegEx()           const { return ui.cbRegEx->isChecked(); }

    QDirIterator* createIterator() const;
    void iterate(QDirIterator* iterator, Actors& actors);   // iterate files and apply actions
    int countFiles();

    void setCurrentDetailModel(TagDetailModel* model);

private:
    Ui::MainWindow ui;

    QString _sourcePath;
    Extractor* _extractor;
    TagFilter* _tagFilter;
    TagCountModel* _tagCountModel;
    TagDetailModel* _tagDetailModel;
    
    QLabel* _labelFileCount;
    QLabel* _labelLineCount;
    QLabel* _labelTagCount;
    QString _filePath;
    QProgressBar* _progressBar;
    TagCounter*   _tagCounter;
};

#endif // MAINWINDOW_H
