#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeModel : public QAbstractItemModel
{
	//Q_OBJECT

public:
	TreeModel(const QStringList *data);
	~TreeModel();

	virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex & child) const override;
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
};

