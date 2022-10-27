#pragma once
#include <QMap>
#include "ui_FilePanelWidget.h"

class QBoxLayout;
class QTabWidget;
class QPushButton;

class FilePanelWidget : public QWidget
{
    Q_OBJECT

public:
    FilePanelWidget(QWidget *parent = Q_NULLPTR);

private:
	QBoxLayout* CreatePanel();
	QAbstractItemView* CreateFileView(QTabWidget* tabWidget);
	void NavigateTo(const QString& dir);
	void NavigateTo(const QModelIndex& index);

private Q_SLOTS:
	void onFileViewDoubleClicked(const QModelIndex& index);
	void onTabBarClicked(int index);

private:
    Ui_FilePanelWidget ui;

	struct Data
	{
		QAbstractItemView* view = nullptr;
	};

	QLineEdit* editNavi_;
	QPushButton* btnUp_;
	QTabWidget* tabWidget_;
	QMap<QAbstractItemModel*, Data> mapModel2Data_;
};
