#include "FilePanelWidget.h"
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

FilePanelWidget::FilePanelWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	auto layout = new QVBoxLayout;
	setLayout(layout);
	layout->addLayout(CreatePanel());
}

QBoxLayout* FilePanelWidget::CreatePanel()
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

QAbstractItemView* FilePanelWidget::CreateFileView(QTabWidget* tabWidget)
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
	mapModel2Data[model].editNavi = nullptr;

	auto icon = QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_DirIcon);
	tabWidget->addTab(view, icon, model->rootDirectory().dirName());

	return view;
}

void FilePanelWidget::NavigateTo(const QString& dir)
{

}

void FilePanelWidget::onFileViewDoubleClicked(const QModelIndex& index)
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

		if (data.editNavi)
		{
			data.editNavi->setText(absDirName);
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

void FilePanelWidget::onTabBarClicked(int index)
{
	auto sendObject = sender();
	auto tabWidget = dynamic_cast<QTabWidget*>(sendObject);
	if (tabWidget == nullptr)
	{
		return;
	}

	CreateFileView(tabWidget);
}

