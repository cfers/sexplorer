#include "sexplorer.h"
#include "FilePanelWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QCloseEvent>

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
	static auto appIcon = QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon);
	this->setWindowIcon(appIcon);
	
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

	// systemtray
     systemTray_ = new QSystemTrayIcon(this);
     systemTray_->setIcon(appIcon);
     systemTray_->setToolTip("welcome to sexplorer!");
	 connect(systemTray_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		 this, SLOT(onSystemTrayActivited(QSystemTrayIcon::ActivationReason)));

     auto restoreAct = new QAction(u8"显示", this);
     connect(restoreAct, SIGNAL(triggered()), this, SLOT(showNormal()));

     auto quitAct = new QAction(u8"关闭", this);
     connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

     auto pContextMenu = new QMenu(this);
     pContextMenu->addAction(restoreAct);
     pContextMenu->addSeparator();
     pContextMenu->addAction(quitAct);
     systemTray_->setContextMenu(pContextMenu);
}

void sexplorer::onSystemTrayActivited(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
	case QSystemTrayIcon::Context:
		break;
	case QSystemTrayIcon::DoubleClick:
		break;
	case QSystemTrayIcon::Trigger: {
		this->showNormal();
	}
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	default:
		break;
	}
}

void sexplorer::closeEvent(QCloseEvent *event)
{
	this->hide();
	systemTray_->show();
	event->ignore();
}
