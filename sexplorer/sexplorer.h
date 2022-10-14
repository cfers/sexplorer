#pragma once
#include <memory>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_sexplorer.h"

class QProcess;
class QBoxLayout;
class QTabWidget;
class sexplorer : public QMainWindow
{
    Q_OBJECT

public:
    sexplorer(QWidget *parent = Q_NULLPTR);

private:
	int64_t NewExplorer();

private:
    Ui::sexplorerClass ui;
	std::vector<std::shared_ptr<QProcess>>	m_vecProcess;
	QBoxLayout* m_tabbarLayout;
	QTabWidget* m_tabwidget;
};
