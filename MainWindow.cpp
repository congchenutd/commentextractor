#include "MainWindow.h"
#include "TextBlock.h"
#include "TagFilter.h"
#include "TagInstanceModel.h"
#include "TagKeywordModel.h"
#include "Counter.h"
#include "ProgressDisplay.h"
#include "FileLog.h"
#include "Extractor.h"
#include "Highlighter.h"
#include "DlgSettings.h"
#include "CommentModel.h"
#include "MyStatusBar.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QProgressBar>
#include <QSettings>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _settings("settings.ini")
{
    ui.setupUi(this);
    loadSettings();

    _labelFileCount      = new QLabel(this);
    _labelLineCount      = new QLabel(this);
    QLabel* labelTagCount     = new QLabel(this);
    QLabel* labelTagLineCount = new QLabel(this);
    QLabel* labelTagLenCount  = new QLabel(this);
    QLabel* labelPackageCount = new QLabel(this);
    QLabel* labelCmtLineCount = new QLabel(this);
    QLabel* labelCmtLenCount  = new QLabel(this);
    _progressBar = new QProgressBar(this);
    _progressBar->hide();
    setStatusBar(new MyStatusBar(this));
    statusBar()->addPermanentWidget(_labelFileCount);
    statusBar()->addPermanentWidget(_labelLineCount);
    statusBar()->addPermanentWidget(labelTagCount);
    statusBar()->addPermanentWidget(labelTagLineCount);
    statusBar()->addPermanentWidget(labelTagLenCount);
    statusBar()->addPermanentWidget(labelPackageCount);
    statusBar()->addPermanentWidget(labelCmtLineCount);
    statusBar()->addPermanentWidget(labelCmtLenCount);
    statusBar()->addPermanentWidget(_progressBar);

    // the counters are updated by the model
    _modelKeywords  = new TagKeywordModel(new TagCounter      (labelTagCount),
                                          new TagLineCounter  (labelTagLineCount),
                                          new TagLengthCounter(labelTagLenCount),
                                          this);
    ui.tvTagKeywords->setModel(_modelKeywords);
//    ui.tvTagKeywords->hideColumn(TagKeywordModel::COL_LINECOUNT);

    _modelComment = new CommentModel(new PackageCounter      (labelPackageCount),
                                     new CommentLineCounter  (labelCmtLineCount),
                                     new CommentLengthCounter(labelCmtLenCount),
                                     this);
    ui.tvComments->setModel(_modelComment);

    _modelInstances = new TagInstanceModel(QString(), this);
    setCurrentInstanceModel(_modelInstances);

    new Highlighter(ui.teTag->document());

    connect(ui.actionExtract,  SIGNAL(triggered()), this, SLOT(onExtract()));
    connect(ui.actionPick,     SIGNAL(triggered()), this, SLOT(onPick()));
    connect(ui.actionLoad,     SIGNAL(triggered()), this, SLOT(onLoad()));
    connect(ui.actionSave,     SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui.actionDelete,   SIGNAL(triggered()), this, SLOT(onDelete()));
    connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(ui.actionExport,   SIGNAL(triggered()), this, SLOT(onExport()));
    connect(ui.tvTagKeywords,  SIGNAL(clicked(QModelIndex)), this, SLOT(onTagKeywordClicked        (QModelIndex)));
    connect(ui.tvTagInstances, SIGNAL(clicked(QModelIndex)), this, SLOT(onTagInstanceClicked(QModelIndex)));
    connect(ui.tvComments,     SIGNAL(clicked(QModelIndex)), this, SLOT(onCommentClicked    (QModelIndex)));
    connect(ui.tvTagKeywords->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTagKeywordClicked(QModelIndex)));
}

void MainWindow::onExtract()
{
    // get project path
    Settings settings;
    QString projectPath = QFileDialog::getExistingDirectory(this, tr("Select the project dir."),
                                                            settings.getLastPath(),
                                                            QFileDialog::ShowDirsOnly);
    if(projectPath.isEmpty())
        return;

    settings.setLastPath(projectPath);
    settings.setProjectPath(projectPath);
    setProjectPath(projectPath);

    if(currentTabIsComment())
        extractComments();
    else
        extractTags();
}

void MainWindow::extractComments()
{
    // reset models
    _modelComment->clear();

    // progress bar
    _progressBar->show();
    _progressBar->setValue(0);
    ProgressBarAdapter progressBarAdapter(_progressBar);
    progressBarAdapter.setMaximum(countFiles());

    // File name display
    FileLog fileLog(statusBar());

    // counters
    FileCounter fileCounter(_labelFileCount);
    LineCounter lineCounter(_labelLineCount);

    // comment extractor extracts all the comments
    Extractor extractor(_settings.getCommentFilter());
    ExtractorAdapter extractorAdapter(&extractor, _modelComment);

    // the iterator feeds files to the actors
    iterate(createIterator(), Actors() << &extractorAdapter
                                       << &fileCounter
                                       << &lineCounter
                                       << &progressBarAdapter
                                       << &fileLog);

    _progressBar->hide();
}

void MainWindow::extractTags()
{
    // reset models
    _modelKeywords->clear();

    // progress bar
    ProgressBarAdapter progressBarAdapter(_progressBar);
    progressBarAdapter.setMaximum(countFiles());
    _progressBar->show();
    _progressBar->setValue(0);

    // File name display
    FileLog fileLog(statusBar());

    // counters
    FileCounter fileCounter(_labelFileCount);
    LineCounter lineCounter(_labelLineCount);

    // comment extractor extracts all the comments
    Extractor extractor(_settings.getCommentFilter());

    // Tag filter finds tags within the comments
    TagFilter tagFilter(&extractor, getContentFilter(), useRegEx());
    TagFilterAdapter tagFilterAdapter(&tagFilter, _modelKeywords);

    // the iterator feeds files to the actors
    iterate(createIterator(), Actors() << &tagFilterAdapter
                                       << &fileCounter
                                       << &lineCounter
                                       << &progressBarAdapter
                                       << &fileLog);

    _progressBar->hide();
}

QDirIterator* MainWindow::createIterator() const {
    return new QDirIterator(Settings().getProjectPath(), getNameFilter(),
                            QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDirIterator::Subdirectories);
}

void MainWindow::iterate(QDirIterator* iterator, Actors& actors)
{
    while(iterator->hasNext()) {
        QString filePath = iterator->next();
        for(Actors::iterator it = actors.begin(); it != actors.end(); ++it)
            (*it)->run(filePath);
    }
}

void MainWindow::onPick()
{
    if(getRandomPickSize() <= 0)
        return;

    _modelKeywords->removeSmall(getRemoveSmallSize());
    _modelKeywords->pick(getRandomPickSize());

    // reset current instance model, because it may have been replaced
    setCurrentInstanceModel(_modelKeywords->getInstanceModel(_modelInstances->getKeyword()));
}

void MainWindow::onLoad()
{
    Settings settings;
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select a dir to load"),
                                                           settings.getLastPath());
    if(!folderPath.isEmpty())
    {
        _modelKeywords->load(folderPath);
        _modelComment ->load(folderPath);
        setProjectPath(settings.getProjectPath());  // the models will have loaded project path
        settings.setLastPath(folderPath);
    }
}

void MainWindow::onSave()
{
    Settings settings;
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select a dir to save to"),
                                                           settings.getLastPath());
    if(!folderPath.isEmpty())
    {
        _modelComment ->save(folderPath);
        _modelKeywords->save(folderPath);
        settings.setLastPath(folderPath);
    }
}

void MainWindow::onTagKeywordClicked(const QModelIndex& idx) {
    if(idx.isValid())
        setCurrentInstanceModel(
                    _modelKeywords->getInstanceModel(
                        _modelKeywords->getKeyword(idx.row())));
}

void MainWindow::onTagInstanceClicked(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    QString filePath = _modelInstances->getFilePath(idx.row());
    int     lineNum  = _modelInstances->getLineNum (idx.row());
    ui.teTag->loadFile(filePath, lineNum);

    statusBar()->showMessage(filePath);
}

void MainWindow::onCommentClicked(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    TextBlock comment = _modelComment->getComment(idx.row());
    QString filePath = comment.getFilePath();
    int     lineNum  = comment.getLineNumber();
    ui.teComment->loadFile(filePath, lineNum);

    statusBar()->showMessage(filePath);
}

void MainWindow::onDelete()
{
    if(currentTabIsComment())
        deleteComments();
    else
        deleteTags();
}

void MainWindow::onExport()
{
    Settings settings;
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export"),
                                                    settings.getLastPath(),
                                                    tr("CSV files (*.csv)"));
    if(!filePath.isEmpty())
    {
        _modelKeywords->exportToFile(filePath, settings.getExportModularity());
        _modelComment    ->exportToFile(filePath, getRandomPickSize());
        settings.setLastPath(QFileInfo(filePath).path());
    }
}

void MainWindow::onSettings()
{
    DlgSettings dlg(this);
    if(dlg.exec() == QDialog::Accepted)
        loadSettings();    // dlg will save the settings, just apply them here
}

int MainWindow::countFiles()
{
    FileCounter fileCounter;
    iterate(createIterator(), Actors() << &fileCounter);
    return fileCounter.getCount();
}

void MainWindow::setCurrentInstanceModel(TagInstanceModel* model)
{
    if(model != 0)
    {
        _modelInstances = model;
        ui.tvTagInstances->setModel(model);
        ui.tvTagInstances->hideColumn(TagInstanceModel::COL_FILEPATH);
        ui.tvTagInstances->hideColumn(TagInstanceModel::COL_LINENUM);
    }
}

void MainWindow::setProjectPath(const QString& projectPath) {
    setWindowTitle(tr("CommentExtractor - ") + projectPath);
}

void MainWindow::deleteTags()
{
    QModelIndexList rows = ui.tvTagKeywords->selectionModel()->selectedRows();
    QStringList toBeRemoved;
    foreach(const QModelIndex& idx, rows)
        toBeRemoved << _modelKeywords->getKeyword(idx.row());
    foreach(const QString& tag, toBeRemoved)
        _modelKeywords->removeKeyword(tag);
}

void MainWindow::deleteComments()
{
    QModelIndexList rows = ui.tvComments->selectionModel()->selectedRows();
    QList<int> toBeRemoved;
    foreach(const QModelIndex& idx, rows)
        toBeRemoved << idx.row();
    qSort(toBeRemoved.begin(), toBeRemoved.end(), qGreater<int>());
    foreach(int row, toBeRemoved)
        _modelComment->removeRow(row);
}

bool MainWindow::currentTabIsComment() const {
    return ui.tabWidget->currentIndex() == 0;
}

void MainWindow::loadSettings()
{
    Settings settings;
    qApp        ->setFont(settings.getUIFont());
    ui.teTag    ->setFont(settings.getEditorFont());
    ui.teComment->setFont(settings.getEditorFont());
    ui.splitterTags    ->restoreState(settings.value("TagSplitterState")    .toByteArray());
    ui.splitterComments->restoreState(settings.value("CommentSplitterState").toByteArray());
}

void MainWindow::saveSettings()
{
    Settings settings;
    settings.setValue("TagSplitterState",     ui.splitterTags    ->saveState());
    settings.setValue("CommentSplitterState", ui.splitterComments->saveState());
}

void MainWindow::closeEvent(QCloseEvent*) {
    saveSettings();
}
