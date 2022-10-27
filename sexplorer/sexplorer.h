#pragma once
#include <memory>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_sexplorer.h"
#include <QSystemTrayIcon>

class QBoxLayout;
class QTabWidget;
class QCloseEvent;

class sexplorer : public QMainWindow
{
    Q_OBJECT

public:
    sexplorer(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
	void onSystemTrayActivited(QSystemTrayIcon::ActivationReason);
	
private:
    Ui::sexplorerClass ui;
	QSystemTrayIcon* systemTray_;
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
