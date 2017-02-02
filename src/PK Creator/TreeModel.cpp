#include "TreeModel.h"

#include <QStringList>

TreeModel::TreeModel(const QStringList *data)
{
	int row = 0;

	for (int i = 0; i < data->size(); ++i)
	{
		printf("Adding %s\n", data->at(i).toStdString().c_str());

		QModelIndex index;
		insertRow(i, index);
		setData(index, QVariant(data->at(i).toStdString().c_str()));
	}
}

TreeModel::~TreeModel()
{

}

QModelIndex TreeModel::index(int row, int column, const QModelIndex & parent) const
{
	return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex & child) const
{
	return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex & parent) const
{
	return 0;
}

int TreeModel::columnCount(const QModelIndex & parent) const
{
	return 0;
}

QVariant TreeModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}
