#include "sexplorer.h"
#include <thread>
#include <QProcess>
#include <assert.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWindow>

#include <windows.h>
typedef struct tagWNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
} WNDINFO, *LPWNDINFO;

//枚举窗口过程
static BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	LPWNDINFO pInfo = (LPWNDINFO)lParam;
	if (dwProcessId == pInfo->dwProcessId)
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

//获取主窗口句柄
static HWND GetProcessMainWnd(DWORD procId)
{
	WNDINFO wi;
	wi.dwProcessId = 18568;
	wi.hWnd = (HWND)0x00040FA6;
	//EnumWindows(EnumProc, (LPARAM)&wi);

	assert(wi.hWnd != NULL);
	return wi.hWnd;
}

sexplorer::sexplorer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	// MAIN LAYOUT
	auto* mainlayout = new QVBoxLayout;
	ui.centralWidget->setLayout(mainlayout);
	
	// TAB BAR
	m_tabbarLayout = new QHBoxLayout;
	auto* pbtn = new QPushButton(u8"new");
	connect(pbtn, &QPushButton::clicked, [this]() { NewExplorer(); });

	m_tabbarLayout->addStretch();
	m_tabbarLayout->addWidget(pbtn);
	mainlayout->addLayout(m_tabbarLayout);
	
	// tabwidget
	m_tabwidget = new QTabWidget;
	mainlayout->addWidget(m_tabwidget);

	mainlayout->addStretch();
}

int64_t sexplorer::NewExplorer()
{
	std::shared_ptr<QProcess> spProc(new QProcess);
	spProc->start("C:/Windows/explorer.exe");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	auto qpid = spProc->pid();
	auto windowHandle = GetProcessMainWnd(qpid->dwProcessId);
	auto tabWindow = QWindow::fromWinId((WId)windowHandle);
	//这边可以设置一下属性
	tabWindow->setFlags(tabWindow->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint); 
	//第二个参数是作为window的父类，也可以用layout
	auto widget = QWidget::createWindowContainer(tabWindow, m_tabwidget);
	m_tabwidget->addTab(widget, u8"explorer");

	m_vecProcess.emplace_back(std::move(spProc));
	return 0;
}
