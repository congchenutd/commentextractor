#include "DlgSettings.h"
#include <QFontDialog>

Settings::Settings(const QString& fileName)
    : QSettings(fileName, QSettings::IniFormat)
{}

QStringList Settings::getNameFilter()       const { return getNameFilterString().split(";"); }
QString     Settings::getNameFilterString() const { return value("NameFilter").toString(); }
QString     Settings::getContentFilter()    const { return value("TagFilter") .toString(); }
int         Settings::getRandomPickSize()   const { return value("RandomPickSize") .toInt(); }
int         Settings::getRemoveSmallSize()  const { return value("RemoveSmallSize").toInt(); }
bool        Settings::useRegEx()            const { return value("UseRegEx").toBool(); }
QByteArray Settings::getSplitterState()     const { return value("SplitterState").toByteArray(); }

QFont Settings::getUIFont() const
{
    QFont font;
    font.fromString(value("UIFont").toString());
    return font;
}

QFont Settings::getEditorFont() const
{
    QFont font;
    font.fromString(value("EditorFont").toString());
    return font;
}


void Settings::setNameFilterString(const QString& filter) { setValue("NameFilter", filter); }
void Settings::setTagFilter       (const QString& filter) { setValue("TagFilter",  filter); }
void Settings::setRandomPickSize  (int size)              { setValue("RandomPickSize",  size); }
void Settings::setRemoveSmallSize (int size)              { setValue("RemoveSmallSize", size); }
void Settings::setUseRegEx        (bool useRegEx)         { setValue("UseRegEx", useRegEx); }
void Settings::setUIFont    (const QFont& font) { setValue("UIFont",     font.toString()); }
void Settings::setEditorFont(const QFont& font) { setValue("EditorFont", font.toString()); }
void Settings::setSplitterState(const QByteArray& state) { setValue("SplitterState", state); }

//////////////////////////////////////////////////////////////////////////////
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
    ui.leNameFilter     ->setText   (settings.getNameFilterString());
    ui.leContentFilter  ->setText   (settings.getContentFilter());
    ui.sbRandomPickSize ->setValue  (settings.getRandomPickSize());
    ui.sbRemoveSmallSize->setValue  (settings.getRemoveSmallSize());
    ui.cbRegEx          ->setChecked(settings.useRegEx());
    setUIFont    (settings.getUIFont());
    setEditorFont(settings.getEditorFont());

}

void DlgSettings::saveSettings()
{
    Settings settings;
    settings.setNameFilterString(getNameFilterString());
    settings.setTagFilter       (getContentFilter());
    settings.setRandomPickSize  (getRandomPickSize());
    settings.setRemoveSmallSize (getRemoveSmallSize());
    settings.setUseRegEx        (useRegEx());
    settings.setUIFont          (getUIFont());
    settings.setEditorFont      (getEditorFont());
}

QString DlgSettings::getNameFilterString() const { return ui.leNameFilter->text().simplified(); }
QString DlgSettings::getContentFilter()    const { return ui.leContentFilter->text(); }
int     DlgSettings::getRandomPickSize()   const { return ui.sbRandomPickSize->value(); }
int     DlgSettings::getRemoveSmallSize()  const { return ui.sbRemoveSmallSize->value(); }
bool    DlgSettings::useRegEx()            const { return ui.cbRegEx->isChecked(); }
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

