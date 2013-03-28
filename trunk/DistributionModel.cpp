#include "DistributionModel.h"

DistributionModel::DistributionModel(QObject* parent) :
    QStandardItemModel(parent)
{
    setColumnCount(1);
    setHeaderData(0, Qt::Horizontal, tr("Package"));
}

int DistributionModel::addPackage(const QString& packageName)
{
    int row = getPackageRow(packageName);
    if(row == -1)
    {
        row = rowCount() - 1;   // the last row is for sum
        insertRow(row);
        setData(index(row, 0), packageName);
    }
    return row;
}

int DistributionModel::addColumn(const QString& keyword)
{
    int col = getKeywordCol(keyword);
    if(col == -1)
    {
        col = columnCount() - 1;  // the last col is for sum
        insertColumn(col);
        setHeaderData(col, Qt::Horizontal, keyword);
    }
    return col;
}

void DistributionModel::addCount(const QString& packageName, const QString& keyword)
{
    int row = addPackage(packageName);
    int col = addColumn(keyword);
    setData(index(row, col), data(index(row, col)).toInt() + 1);
}

int DistributionModel::getPackageRow(const QString& packageName) const
{
    for(int row = 0; row < rowCount(); ++row)
        if(data(index(row, 0)).toString() == packageName)
            return row;
    return -1;
}

int DistributionModel::getKeywordCol(const QString& keyword) const
{
    for(int col = 0; col < columnCount(); ++col)
        if(headerData(col, Qt::Horizontal).toString() == keyword)
            return col;
    return -1;
}

void DistributionModel::updateSum()
{
}

