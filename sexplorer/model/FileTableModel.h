#include <QFileSystemModel>

class FileTableModel : public QFileSystemModel
{

public:
	using QFileSystemModel::QFileSystemModel;

	void SetRootIndex();
};