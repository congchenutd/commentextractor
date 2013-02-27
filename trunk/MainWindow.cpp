#include "MainWindow.h"
#include "TextBlock.h"
#include "TagFilter.h"
#include "TagCountModel.h"
#include "TagDetailModel.h"
#include "Counter.h"
#include "ProgressDisplay.h"
#include "FileLog.h"
#include "Writer.h"
#include "Extractor.h"
#include "Highlighter.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QProgressBar>
#include <QSettings>
#include <QTextStream>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    loadSettings();

    _labelFileCount = new QLabel(this);
    _labelLineCount = new QLabel(this);
    _labelTagCount  = new QLabel(this);
    _progressBar = new QProgressBar(this);
    statusBar()->addPermanentWidget(_labelFileCount);
    statusBar()->addPermanentWidget(_labelLineCount);
    statusBar()->addPermanentWidget(_labelTagCount);
    statusBar()->addPermanentWidget(_progressBar);
    _progressBar->hide();

    ui.splitter->setSizes(QList<int>() << 100 << 200 << 300);

    _tagCountModel  = new TagCountModel (this);
    _tagDetailModel = new TagDetailModel(QString(), this);

    _tagCounter = new TagCounter(_labelTagCount);
    _tagCountModel->setCounter(_tagCounter);

    ui.tvTagCount->setModel(_tagCountModel);
    ui.tvTagCount->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setCurrentDetailModel(_tagDetailModel);

    new Highlighter(ui.textEdit->document());

    connect(ui.actionExtract, SIGNAL(triggered()), this, SLOT(onExtract()));
    connect(ui.actionPick,    SIGNAL(triggered()), this, SLOT(onPick()));
    connect(ui.actionOpen,    SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui.actionSave,    SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui.actionDelete,  SIGNAL(triggered()), this, SLOT(onDeleteTag()));
    connect(ui.tvTagCount,  SIGNAL(clicked(QModelIndex)), this, SLOT(onTagClicked(QModelIndex)));
    connect(ui.tvTagDetail, SIGNAL(clicked(QModelIndex)), this, SLOT(onTagDetailClicked(QModelIndex)));
    connect(ui.tvTagCount->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTagClicked(QModelIndex)));
}

void MainWindow::closeEvent(QCloseEvent*) {
    saveSettings();
}

void MainWindow::onExtract()
{
    _projectPath = QDir::toNativeSeparators(
                QFileDialog::getExistingDirectory(this, tr("Select dir"), "."));
    if(_projectPath.isEmpty())
        return;

    _tagCountModel->clear();

    // progress actor
    ProgressDisplay progressChecker(_progressBar);
    progressChecker.setMaximum(countFiles());
    _progressBar->show();
    _progressBar->setValue(0);

    // File name display
    FileLog fileLog(statusBar());

    // file and line counters
    FileCounter fileCounter(_labelFileCount);
    LineCounter lineCounter(_labelLineCount);
    _tagCounter->reset();

    // extractor and filter
    Extractor extractor("(/\\*([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+/)|//[^\\r\\n]*");
    TagFilter tagFilter(&extractor, _tagCountModel);
    tagFilter.setFilter(getTagFilter(), useRegEx());

    // apply actors on the files
    iterate(createIterator(), Actors() << &tagFilter << &fileCounter
                                       << &lineCounter << &progressChecker << &fileLog);

    _progressBar->hide();
}

void MainWindow::onPick()
{
    if(getRandomPickSize() <= 0)
        return;
    _tagCountModel->removeSmall(getRemoveSmallSize());
    _tagCountModel->pick(getRandomPickSize());
    setCurrentDetailModel(_tagCountModel->getDetail(_tagDetailModel->getTag()));
}

void MainWindow::onOpen()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select path"), ".");
    if(!folderPath.isEmpty())
        _tagCountModel->load(folderPath);
}

void MainWindow::onSave()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select path"), ".");
    if(!folderPath.isEmpty())
        _tagCountModel->save(folderPath, _projectPath);
}

void MainWindow::onTagClicked(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    TagDetailModel* model = _tagCountModel->getDetail(
                                 _tagCountModel->data(
                                     _tagCountModel->index(idx.row(),
                                                           TagCountModel::COL_TAG)).toString());
    setCurrentDetailModel(model);
}

void MainWindow::onDeleteTag()
{
    QModelIndexList rows = ui.tvTagCount->selectionModel()->selectedRows();
    QStringList toBeRemoved;
    foreach(const QModelIndex& idx, rows)
        toBeRemoved << _tagCountModel->data(
                           _tagCountModel->index(idx.row(),
                                                 TagCountModel::COL_TAG)).toString();
    foreach(const QString& tag, toBeRemoved)
        _tagCountModel->removeTag(tag);
}

void MainWindow::onTagDetailClicked(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    QString filePath = _tagDetailModel->data(_tagDetailModel->index(idx.row(), TagDetailModel::COL_FILEPATH)).toString();
    int     line     = _tagDetailModel->data(_tagDetailModel->index(idx.row(), TagDetailModel::COL_LINENUM)).toInt();
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
        return;

    statusBar()->showMessage(filePath);

    QTextStream os(&file);
    ui.textEdit->setPlainText(os.readAll());
    ui.textEdit->verticalScrollBar()->setValue(ui.textEdit->verticalScrollBar()->maximum());
    QTextBlock block = ui.textEdit->document()->findBlockByLineNumber(line - 1);
    QTextCursor cursor(block);
    ui.textEdit->setTextCursor(cursor);
    ui.textEdit->ensureCursorVisible();

    QTextEdit::ExtraSelection highlight;
    highlight.cursor = cursor;
    highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
    highlight.format.setBackground(Qt::yellow);
    ui.textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>() << highlight);
}

QDirIterator* MainWindow::createIterator() const {
    return new QDirIterator(_projectPath, getNameFilter(),
                            QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                            QDirIterator::Subdirectories);
}

void MainWindow::iterate(QDirIterator* iterator, Actors& actors)
{
    while(iterator->hasNext())
    {
        QString filePath = iterator->next();
        for(Actors::iterator it = actors.begin(); it != actors.end(); ++it)
            (*it)->run(filePath);
    }
}

int MainWindow::countFiles()
{
    FileCounter fileCounter;
    iterate(createIterator(), Actors() << &fileCounter);
    return fileCounter.getCount();
}

void MainWindow::setCurrentDetailModel(TagDetailModel* model)
{
    if(model != 0)
    {
        _tagDetailModel = model;
        ui.tvTagDetail->setModel(model);
        ui.tvTagDetail->hideColumn(TagDetailModel::COL_FILEPATH);
        ui.tvTagDetail->hideColumn(TagDetailModel::COL_LINENUM);
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    ui.leNameFilter->setText(settings.value("NameFilter").toString());
    ui.leTagFilter ->setText(settings.value("TagFilter") .toString());
    ui.sbRandomPickSize ->setValue(settings.value("RandomPickSize") .toInt());
    ui.sbRemoveSmallSize->setValue(settings.value("RemoveSmallSize").toInt());
    ui.cbRegEx->setChecked(settings.value("UseRegEx").toBool());
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("NameFilter",      ui.leNameFilter->text());
    settings.setValue("TagFilter",       getTagFilter());
    settings.setValue("RandomPickSize",  getRandomPickSize());
    settings.setValue("RemoveSmallSize", getRemoveSmallSize());
    settings.setValue("UseRegEx",        useRegEx());
}
