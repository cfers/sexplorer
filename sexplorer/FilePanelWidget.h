#pragma once
#include <memory>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_FilePanelWidget.h"

class QBoxLayout;
class QTabWidget;
class FilePanelWidget : public QWidget
{
    Q_OBJECT

public:
    FilePanelWidget(QWidget *parent = Q_NULLPTR);

private:
	QBoxLayout* CreatePanel();
	QAbstractItemView* CreateFileView(QTabWidget* tabWidget);
	void NavigateTo(const QString& dir);

private Q_SLOTS:
	void onFileViewDoubleClicked(const QModelIndex& index);
	void onTabBarClicked(int index);

private:
    Ui_FilePanelWidget ui;

	struct Data
	{
		QAbstractItemView* view = nullptr;
		QLineEdit*	editNavi = nullptr;
	};

	QMap<QAbstractItemModel*, Data> mapModel2Data;
};
