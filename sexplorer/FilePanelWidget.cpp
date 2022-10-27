#include "FilePanelWidget.h"
#include "model/FileTableModel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QLineEdit>
#include <QPushButton>

FilePanelWidget::FilePanelWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	auto layout = new QVBoxLayout;
	layout->setSpacing(1);
	layout->setMargin(2);
	setLayout(layout);
	layout->addLayout(CreatePanel());
}

QBoxLayout* FilePanelWidget::CreatePanel()
{
	// navigate bar
	QHBoxLayout* naviLayout = new QHBoxLayout;
	
	static auto iconUp = QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_ArrowUp);
	btnUp_ = new QPushButton(iconUp, "");
	naviLayout->addWidget(btnUp_);

	editNavi_ = new QLineEdit;
	editNavi_->setText(qApp->applicationDirPath());
	connect(editNavi_, &QLineEdit::editingFinished, [&] 
	{
		NavigateTo(editNavi_->text());
	});
	naviLayout->addWidget(editNavi_);

	connect(btnUp_, &QPushButton::clicked, [this] 
	{
		QString dir = editNavi_->text();
		dir.truncate(dir.lastIndexOf('/'));
		NavigateTo(dir);
	});

	// tabwidget
	tabWidget_ = new QTabWidget;
	CreateFileView(tabWidget_);
	connect(tabWidget_, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabBarClicked(int)));

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setSpacing(2);
	layout->addLayout(naviLayout);
	layout->addWidget(tabWidget_);
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

	mapModel2Data_[model].view = view;

	auto icon = QApplication::style()->standardIcon(QStyle::StandardPixmap::SP_DirIcon);
	tabWidget->addTab(view, icon, model->rootDirectory().dirName());

	return view;
}
void FilePanelWidget::NavigateTo(const QString& path)
{
	if(!QDir(path).exists())
	{
		return;
	}

	auto curWidget = tabWidget_->currentWidget();
	QAbstractItemView* view = dynamic_cast<QAbstractItemView*>(curWidget);
	if (view == nullptr)
	{
		return;
	}

	auto fileSysModel = dynamic_cast<QFileSystemModel*>(view->model());
	if (fileSysModel == nullptr)
	{
		return;
	}

	view->setRootIndex(fileSysModel->setRootPath(path));

	bool isFile = false;
	QFileInfo finfo(path);
	QString absDirName = finfo.absoluteFilePath();
	if (!finfo.isDir())
	{
		isFile = true;
		absDirName = finfo.absolutePath();
	}

	editNavi_->setText(absDirName);

	if (!isFile)
	{
		auto curDirName = QDir(path).dirName();
		auto tab = dynamic_cast<QTabWidget*>(view->parent()->parent());
		if (tab != nullptr)
		{
			tab->setTabText(tab->indexOf(view), curDirName);
		}
	}

	// change dir
	editNavi_->setText(path);
}

void FilePanelWidget::NavigateTo(const QModelIndex& index)
{
	auto cmodel = index.model();
	auto model = const_cast<QAbstractItemModel*>(cmodel);
	auto fileSysModel = dynamic_cast<QFileSystemModel*>(model);
	if (!mapModel2Data_.contains(model) || fileSysModel == nullptr)
	{
		return;
	}

	auto path = fileSysModel->filePath(index);

	NavigateTo(path);
}

void FilePanelWidget::onFileViewDoubleClicked(const QModelIndex& index)
{
	NavigateTo(index);
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

