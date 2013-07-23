#include "Settings.h"
#include <QStringList>
#include <QFont>

Settings::Settings(const QString& fileName)
    : QSettings(fileName, QSettings::IniFormat)
{}

QStringList Settings::getNameFilter()       const { return getNameFilterString().split(";"); }
QString     Settings::getNameFilterString() const { return value("NameFilter").toString(); }
QString     Settings::getContentFilter()    const { return value("TagFilter") .toString(); }
int         Settings::getRandomPickSize()   const { return value("RandomPickSize") .toInt(); }
int         Settings::getRemoveSmallSize()  const { return value("RemoveSmallSize").toInt(); }
bool        Settings::useRegEx()            const { return value("UseRegEx").toBool(); }
//QByteArray  Settings::getSplitterState()    const { return value("SplitterState").toByteArray(); }
QString     Settings::getLastPath()         const { return value("LastPath").toString(); }
QString     Settings::getExportModularity() const { return value("ExportModularity").toString().toUpper(); }

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
//void Settings::setSplitterState(const QByteArray& state) { setValue("SplitterState", state); }
void Settings::setLastPath(const QString& path) { setValue("LastPath", path); }
void Settings::setExportModularity(const QString& modularity) { setValue("ExportModularity", modularity); }
