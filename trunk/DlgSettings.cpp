#include "DlgSettings.h"
#include "Settings.h"
#include <QFontDialog>

DlgSettings::DlgSettings(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    loadSettings();

    connect(ui.btUIFont,     SIGNAL(clicked()), this, SLOT(onUIFont()));
    connect(ui.btEditorFont, SIGNAL(clicked()), this, SLOT(onEditorFont()));
}

void DlgSettings::accept() {
    saveSettings();
    QDialog::accept();
}

void DlgSettings::loadSettings()
{
    Settings settings;
    ui.leNameFilter   ->setText(settings.getNameFilterString());
    ui.leCommentFilter->setText(settings.getCommentFilter());
    ui.leTagFilter    ->setText(settings.getTagFilter());
    ui.sbRandomPickSize ->setValue  (settings.getRandomPickSize());
    ui.sbRemoveSmallSize->setValue  (settings.getRemoveSmallSize());
    ui.checkRegEx       ->setChecked(settings.useRegEx());
    setExportModularity(settings.getExportModularity());
    setUIFont    (settings.getUIFont());
    setEditorFont(settings.getEditorFont());
}

void DlgSettings::saveSettings()
{
    Settings settings;
    settings.setNameFilterString(getNameFilterString());
    settings.setCommentFilter   (getCommentFilter());
    settings.setTagFilter       (getTagFilter());
    settings.setRandomPickSize  (getRandomPickSize());
    settings.setRemoveSmallSize (getRemoveSmallSize());
    settings.setUseRegEx        (useRegEx());
    settings.setUIFont          (getUIFont());
    settings.setEditorFont      (getEditorFont());
    settings.setExportModularity(getExportModularity());
}

QString DlgSettings::getExportModularity() const {
    return ui.radioClass->isChecked() ? "CLASS" :
           ui.radioFile ->isChecked() ? "FILE" :
                                        "PACKAGE";
}

void DlgSettings::setExportModularity(const QString& modularity)
{
    ui.radioClass  ->setChecked(modularity == "CLASS");
    ui.radioFile   ->setChecked(modularity == "FILE");
    ui.radioPackage->setChecked(modularity == "PACKAGE");
}

QString DlgSettings::getNameFilterString() const { return ui.leNameFilter->text().simplified(); }
QString DlgSettings::getCommentFilter()    const { return ui.leCommentFilter->text(); }
QString DlgSettings::getTagFilter()        const { return ui.leTagFilter->text(); }
int     DlgSettings::getRandomPickSize()   const { return ui.sbRandomPickSize->value(); }
int     DlgSettings::getRemoveSmallSize()  const { return ui.sbRemoveSmallSize->value(); }
bool    DlgSettings::useRegEx()            const { return ui.checkRegEx->isChecked(); }
QFont   DlgSettings::getUIFont()           const { return ui.btUIFont->font(); }
QFont   DlgSettings::getEditorFont()       const { return ui.btEditorFont->font(); }

void DlgSettings::setUIFont    (const QFont& font) { ui.btUIFont->setFont(font);     }
void DlgSettings::setEditorFont(const QFont& font) { ui.btEditorFont->setFont(font); }

void DlgSettings::onUIFont()
{
    QFont font = getUIFont();
    bool ok;
    font = QFontDialog::getFont(&ok, font, this);
    if (ok)
        ui.btUIFont->setFont(font);
}

void DlgSettings::onEditorFont()
{
    QFont font = getEditorFont();
    bool ok;
    font = QFontDialog::getFont(&ok, font, this);
    if (ok)
        ui.btEditorFont->setFont(font);
}

