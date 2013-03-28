#ifndef DISTRIBUTIONMODEL_H
#define DISTRIBUTIONMODEL_H

#include <QStandardItemModel>

class DistributionModel : public QStandardItemModel
{
public:
    explicit DistributionModel(QObject *parent = 0);
    
    int  addPackage(const QString& packageName);  // return the row of the package
    void addCount  (const QString& packageName, const QString& keyword);

private:
    int getPackageRow(const QString& packageName) const;
    int getKeywordCol(const QString& keyword)     const;
    int addColumn    (const QString& keyword);
    void updateSum();
};

#endif // DISTRIBUTIONMODEL_H
