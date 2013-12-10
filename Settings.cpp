#include "Settings.h"
#include <QStringList>
#include <QFont>
#include <QDir>

Settings::Settings(const QString& fileName)
    : QSettings(fileName, QSettings::IniFormat)
{}

QStringList Settings::getNameFilter()   const { return getNameFilterString().split(";"); }
QString Settings::getCommentFilter()    const { return value("CommentFilter").toString(); }
QString Settings::getNameFilterString() const { return value("NameFilter").toString(); }
QString Settings::getTagFilter()        const { return value("TagFilter") .toString(); }
int     Settings::getRandomPickSize()   const { return value("RandomPickSize") .toInt(); }
int     Settings::getRemoveSmallSize()  const { return value("RemoveSmallSize").toInt(); }
bool    Settings::useRegEx()            const { return value("UseRegEx").toBool(); }
QString Settings::getLastPath()         const { return value("LastPath")   .toString(); }
QString Settings::getProjectPath()      const { return QDir::toNativeSeparators(value("ProjectPath").toString()); }
QString Settings::getExportModularity() const { return value("ExportModularity").toString().toUpper(); }

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

void Settings::setNameFilterString(const QString& filter) { setValue("NameFilter",    filter); }
void Settings::setCommentFilter   (const QString& filter) { setValue("CommentFilter", filter); }
void Settings::setTagFilter       (const QString& filter) { setValue("TagFilter",     filter); }
void Settings::setRandomPickSize  (int size)              { setValue("RandomPickSize",  size); }
void Settings::setRemoveSmallSize (int size)              { setValue("RemoveSmallSize", size); }
void Settings::setUseRegEx        (bool useRegEx)         { setValue("UseRegEx", useRegEx); }
void Settings::setUIFont    (const QFont& font) { setValue("UIFont",     font.toString()); }
void Settings::setEditorFont(const QFont& font) { setValue("EditorFont", font.toString()); }
void Settings::setLastPath   (const QString& path) { setValue("LastPath",    path); }
void Settings::setProjectPath(const QString& path) { setValue("ProjectPath", QDir::toNativeSeparators(path)); }
void Settings::setExportModularity(const QString& modularity) { setValue("ExportModularity", modularity); }
