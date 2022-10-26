﻿#include "sexplorer.h"
#include "model/FileTableModel.h"
#include <assert.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDirModel>
#include <QModelIndex>
#include <QLineEdit>
#include <QPushButton>

sexplorer::sexplorer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	// set icon
	this->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
	
	// main layout
	auto* mainlayout = new QVBoxLayout;
	ui.centralWidget->setLayout(mainlayout);
	
	// tabwidget
	auto tabPanelLayout = new QHBoxLayout;
	tabPanelLayout->setSpacing(1);
	tabPanelLayout->addLayout(CreatePanel(), 1);
	tabPanelLayout->addLayout(CreatePanel(), 1);
	mainlayout->addLayout(tabPanelLayout);
	
	// resize
	resize(1366, 768);
}

QBoxLayout* sexplorer::CreatePanel()
{
	// navigate bar
	QHBoxLayout* naviLayout = new QHBoxLayout;
	QLineEdit* edit = new QLineEdit;
	QPushButton* btnUp = new QPushButton(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_ArrowUp), "");
	naviLayout->addWidget(btnUp);
	naviLayout->addWidget(edit);

	QTabWidget* tabwidget = new QTabWidget;
	CreateFileView(tabwidget);
	connect(tabwidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabBarClicked(int)));

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setSpacing(2);
	layout->addLayout(naviLayout);
	layout->addWidget(tabwidget);
	return layout;
}

QAbstractItemView* sexplorer::CreateFileView(QTabWidget* tabWidget)
{
	auto* view = new QTreeView;
	auto model = new FileTableModel;
	view->setModel(model);
	view->setRootIndex(model->setRootPath(qApp->applicationDirPath()));
	view->setColumnWidth(0, 300);
	view->setAlternatingRowColors(true);
	view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	connect(view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onFileViewDoubleClicked(const QModelIndex&)));

	mapModel2Data[model].view = view;
	mapModel2Data[model].editCurDir = nullptr;

	auto icon = QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_DirIcon);
	tabWidget->addTab(view, icon, model->rootDirectory().dirName());

	return view;
}

void sexplorer::NavigateTo(const QString& dir)
{

}

void sexplorer::onFileViewDoubleClicked(const QModelIndex& index)
{
	auto cmodel = index.model();
	if (cmodel == nullptr)
	{
		return;
	}
	
	auto model = const_cast<QAbstractItemModel*>(cmodel);
	auto fileSysModel = dynamic_cast<QFileSystemModel*>(model);
	if (fileSysModel == nullptr)
	{
		return;
	}

	if (mapModel2Data.contains(model))
	{
		auto& data = mapModel2Data.value(model);
		auto view = data.view;
		auto path = fileSysModel->filePath(index);
		view->setRootIndex(fileSysModel->setRootPath(path));
	
		bool isFile = false;
		QFileInfo finfo(path);
		QString absDirName = finfo.absoluteFilePath();
		if (!finfo.isDir())
		{
			isFile = true;
			absDirName = finfo.absolutePath();
		}

		if (data.editCurDir)
		{
			data.editCurDir->setText(absDirName);
		}

		if (!isFile)
		{
			auto curDirName = QDir(path).dirName();
			auto tab = dynamic_cast<QTabWidget*>(view->parent()->parent());
			if (tab != nullptr)
			{
				tab->setTabText(tab->indexOf(view), curDirName);
			}
		}
	}
}

void sexplorer::onTabBarClicked(int index)
{
	auto sendObject = sender();
	auto tabWidget = dynamic_cast<QTabWidget*>(sendObject);
	if (tabWidget == nullptr)
	{
		return;
	}

	CreateFileView(tabWidget);
}

