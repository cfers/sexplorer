#include "sexplorer.h"
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
	
	// main layout
	auto* mainlayout = new QVBoxLayout;
	ui.centralWidget->setLayout(mainlayout);
	
	// tabwidget
	auto tabPanelLayout = new QHBoxLayout;
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
	naviLayout->addWidget(new QPushButton(QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight), ""));
	naviLayout->addWidget(edit);

	QTabWidget* tabwidget = new QTabWidget;
	
	auto icon = QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_DirIcon);
	for (int i = 0; i < 10; i++)
	{
		auto model = new FileTableModel;
		model->setRootPath(qApp->applicationDirPath());
		
		auto* view = new QTreeView;
		connect(view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onTableDoubleClicked(const QModelIndex&)));
		view->setModel(model);

		view->setColumnWidth(0, 300);
		view->setAlternatingRowColors(true);
		view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

		mapModel2View[model] = view;
		tabwidget->addTab(view, icon, QString(u8"窗口%1").arg(i));
	}

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(naviLayout);
	layout->addWidget(tabwidget);
	return layout;
}

void sexplorer::onTableDoubleClicked(const QModelIndex& index)
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

	if (mapModel2View.contains(model))
	{
		auto view = mapModel2View.value(model);
		auto path = fileSysModel->filePath(index);
		view->setRootIndex(fileSysModel->setRootPath(path));
		
		auto curDirName = QDir(path).dirName();
		auto tab = dynamic_cast<QTabWidget*>(view->parent()->parent());
		if (tab != nullptr)
		{
			tab->setTabText(tab->indexOf(view), curDirName);
		}
	}
}

