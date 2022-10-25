#pragma once
#include <memory>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_sexplorer.h"

class QProcess;
class QBoxLayout;
class QTabWidget;
class sexplorer : public QMainWindow
{
    Q_OBJECT

public:
    sexplorer(QWidget *parent = Q_NULLPTR);

private:
	QBoxLayout* CreatePanel();

private Q_SLOTS:
	void onTableDoubleClicked(const QModelIndex& index);

private:
    Ui::sexplorerClass ui;
	QBoxLayout* m_tabbarLayout;
	QTabWidget* m_tabwidgetLeft;
	QTabWidget* m_tabwidgetRight;
	QMap<QAbstractItemModel*, QAbstractItemView*> mapModel2View;
};
