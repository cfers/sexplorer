#include "sexplorer.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	// load qss
	QFile f(":/sexplorer/res/blank0.qss");
	if (f.open(QFile::ReadOnly | QFile::Text))
	{
		QString qss = f.readAll();
		a.setStyleSheet(qss);
		f.close();
	}

    sexplorer w;
    w.show();
    return a.exec();
}
