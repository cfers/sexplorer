#include "sexplorer.h"
#include <assert.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>

sexplorer::sexplorer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	// main layout
	auto* mainlayout = new QVBoxLayout;
	ui.centralWidget->setLayout(mainlayout);
	
	// tabwidget
	auto tabwidgetLayout = new QHBoxLayout;
	m_tabwidgetLeft = CreateTabWidget();
	m_tabwidgetRight = CreateTabWidget();
	tabwidgetLayout->addWidget(m_tabwidgetLeft, 1);
	tabwidgetLayout->addWidget(m_tabwidgetRight, 1);
	mainlayout->addLayout(tabwidgetLayout);
	
	// resize
	resize(1366, 768);
}

QTabWidget* sexplorer::CreateTabWidget()
{
	QTabWidget* tabwidget = new QTabWidget;
	
	auto icon = QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_DirIcon);
	for (int i = 0; i < 10; i++)
	{
		auto model = new QFileSystemModel;
		model->setRootPath(qApp->applicationDirPath());
		
		auto* view = new QTreeView;
		view->setModel(model);

		tabwidget->addTab(view, icon, QString(u8"窗口%1").arg(i));
	}

	return tabwidget;
}

