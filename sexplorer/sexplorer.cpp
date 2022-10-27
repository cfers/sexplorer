#include "sexplorer.h"
#include "FilePanelWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>

sexplorer::sexplorer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.statusBar->setMaximumHeight(10);

	// load qss
	QFile f(":/sexplorer/res/blank-1.qss");
	if (f.open(QFile::ReadOnly | QFile::Text))
	{
		QString qss = f.readAll();
		setStyleSheet(qss);
		f.close();
	}

	// set icon
	this->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon));
	
	// main layout
	auto* mainlayout = new QVBoxLayout;
	ui.centralWidget->setLayout(mainlayout);
	
	// filepanel widget
	auto filePanelsLayout = new QHBoxLayout;
	filePanelsLayout->addWidget(new FilePanelWidget);
	filePanelsLayout->addWidget(new FilePanelWidget);
	filePanelsLayout->setSpacing(1);
	mainlayout->addLayout(filePanelsLayout);
	
	// resize
	resize(1366, 768);
}