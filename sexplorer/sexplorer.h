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
	QAbstractItemView* CreateFileView(QTabWidget* tabWidget);
	void NavigateTo(const QString& dir);

private Q_SLOTS:
	void onFileViewDoubleClicked(const QModelIndex& index);
	void onTabBarClicked(int index);

private:
    Ui::sexplorerClass ui;
	QBoxLayout* m_tabbarLayout;

	struct Data
	{
		QAbstractItemView* view = nullptr;
		QLineEdit*	editCurDir = nullptr;
	};

	QMap<QAbstractItemModel*, Data> mapModel2Data;
};
