#ifndef MODULEMODEL_H
#define MODULEMODEL_H

#include <QStandardItemModel>

class ModuleModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ModuleModel(QObject* parent = 0);
    
signals:
    
public slots:
    
};

#endif // MODULEMODEL_H
