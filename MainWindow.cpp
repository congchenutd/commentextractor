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
#include "DlgSettings.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QProgressBar>
#include <QSettings>
#include <QTextStream>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _settings("settings.ini")
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

    _tagCounter = new TagCounter(_labelTagCount);
    _modelCount  = new TagCountModel(this, _tagCounter);
    _modelInstances = new TagInstanceModel(QString(), this);

    ui.tvTagCount->setModel(_modelCount);
    ui.tvTagCount->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setCurrentInstanceModel(_modelInstances);

    new Highlighter(ui.textEdit->document());

    connect(ui.actionExtract,  SIGNAL(triggered()), this, SLOT(onExtract()));
    connect(ui.actionPick,     SIGNAL(triggered()), this, SLOT(onPick()));
    connect(ui.actionLoad,     SIGNAL(triggered()), this, SLOT(onLoad()));
    connect(ui.actionSave,     SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui.actionDelete,   SIGNAL(triggered()), this, SLOT(onDeleteTag()));
    connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(ui.actionExport,   SIGNAL(triggered()), this, SLOT(onExport()));
    connect(ui.tvTagCount,  SIGNAL(clicked(QModelIndex)), this, SLOT(onTagClicked(QModelIndex)));
    connect(ui.tvInstances, SIGNAL(clicked(QModelIndex)), this, SLOT(onTagInstanceClicked(QModelIndex)));
    connect(ui.tvTagCount->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTagClicked(QModelIndex)));
}

void MainWindow::onExtract()
{
    // get project path
    QString projectPath = QFileDialog::getExistingDirectory(this, tr("Select a dir to extract"), ".");
    if(projectPath.isEmpty())
        return;

    setProjectPath(projectPath);
    _modelCount->setProjectPath(projectPath);
    _modelCount->clear();

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
    TagFilter tagFilter(&extractor, _modelCount);
    tagFilter.setFilter(getContentFilter(), useRegEx());

    // apply actors on the files
    iterate(createIterator(), Actors() << &tagFilter << &fileCounter
                                       << &lineCounter << &progressChecker << &fileLog);

    _progressBar->hide();
}

void MainWindow::onPick()
{
    if(getRandomPickSize() <= 0)
        return;
    _modelCount->removeSmall(getRemoveSmallSize());
    _modelCount->pick(getRandomPickSize());

    // reset current instance model, because it may have been replaced
    setCurrentInstanceModel(_modelCount->getInstanceModel(_modelInstances->getKeyword()));
}

void MainWindow::onLoad()
{
    Settings settings;
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select a dir to load"), settings.getLastPath());
    if(!folderPath.isEmpty())
    {
        _modelCount->load(folderPath);
        setProjectPath(_modelCount->getProjectPath());  // count model will have loaded project path
        settings.setLastPath(folderPath);
    }
}

void MainWindow::onSave()
{
    Settings settings;
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select a dir to save to"), settings.getLastPath());
    if(!folderPath.isEmpty())
    {
        _modelCount->save(folderPath);
        settings.setLastPath(folderPath);
    }
}

void MainWindow::onTagClicked(const QModelIndex& idx) {
    if(idx.isValid())
        setCurrentInstanceModel(_modelCount->getInstanceModel(_modelCount->getKeyword(idx.row())));
}

void MainWindow::onTagInstanceClicked(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    QString filePath = _modelInstances->getFilePath(idx.row());
    int     lineNum  = _modelInstances->getLineNum (idx.row());
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
        return;

    statusBar()->showMessage(filePath);

    // read file
    QTextStream os(&file);
    ui.textEdit->setPlainText(os.readAll());

    // scroll to bottom so that the line will be shown at the top of the page
    ui.textEdit->verticalScrollBar()->setValue(ui.textEdit->verticalScrollBar()->maximum());

    // find the block containing lineNum, and move cursor to the block
    QTextBlock block = ui.textEdit->document()->findBlockByLineNumber(lineNum - 1);
    QTextCursor cursor(block);
    ui.textEdit->setTextCursor(cursor);
    ui.textEdit->ensureCursorVisible();

    // highlight the line
    QTextEdit::ExtraSelection highlight;
    highlight.cursor = cursor;
    highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
    highlight.format.setBackground(Qt::yellow);
    ui.textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>() << highlight);
}

void MainWindow::onDeleteTag()
{
    QModelIndexList rows = ui.tvTagCount->selectionModel()->selectedRows();
    QStringList toBeRemoved;
    foreach(const QModelIndex& idx, rows)
        toBeRemoved << _modelCount->getKeyword(idx.row());
    foreach(const QString& tag, toBeRemoved)
        _modelCount->remove(tag);
}

void MainWindow::onExport()
{
    Settings settings;
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export"),
                                                    settings.getLastPath(),
                                                    tr("CSV files (*.csv)"));
    if(!filePath.isEmpty())
    {
        _modelCount->exportToFile(filePath);
        settings.setLastPath(QFileInfo(filePath).path());
    }
}

void MainWindow::onSettings()
{
    DlgSettings dlg(this);
    if(dlg.exec() == QDialog::Accepted)
        loadSettings();
}

QDirIterator* MainWindow::createIterator() const {
    return new QDirIterator(_modelCount->getProjectPath(), getNameFilter(),
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
        ui.tvInstances->setModel(model);
        ui.tvInstances->hideColumn(TagInstanceModel::COL_FILEPATH);
        ui.tvInstances->hideColumn(TagInstanceModel::COL_LINENUM);
    }
}

void MainWindow::setProjectPath(const QString& projectPath) {
    setWindowTitle(tr("CommentExtractor - ") + projectPath);
}

void MainWindow::loadSettings()
{
    Settings settings;
    qApp       ->setFont(settings.getUIFont());
    ui.textEdit->setFont(settings.getEditorFont());
    ui.splitter->restoreState(settings.getSplitterState());
}

void MainWindow::saveSettings()
{
    Settings settings;
    settings.setSplitterState(ui.splitter->saveState());
}

void MainWindow::closeEvent(QCloseEvent*) {
    saveSettings();
}
