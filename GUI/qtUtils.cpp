#include "qtUtils.h"

#include <QFileDialog>
#include "common/toolsOperation.h"

bool openFileSelectionDialog(std::string & filename,
                             const std::string & caption,
                             const std::string & filter,
                             QWidget *parentWidget)
{
    //QString qtFilename = QFileDialog::getOpenFileName(nullptr,
    /*QString qtFilename = QFileDialog::getOpenFileName(parentWidget,
                                                      caption.c_str(),
                                                      ".",
                                                      filter.c_str());*/
    QString qtFilename;

    QFileDialog dialog(parentWidget);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(filter.c_str());
    dialog.setViewMode(QFileDialog::Detail);
    //dialog.setOption(QFileDialog::DontUseNativeDialog);
    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        qtFilename = fileNames[0];
        filename = qtFilename.toStdString();
    }

    return (!qtFilename.isEmpty());
}

bool openFileSaveDialog(std::string & filename,
                        const std::string & caption,
                        const std::string & filter,
                        QWidget *parentWidget)
{
    QString qtFilename = QFileDialog::getSaveFileName(parentWidget,
                                                      caption.c_str(),
                                                      QString(),
                                                      filter.c_str(),
                                                      Q_NULLPTR/*,
                                                      QFileDialog::DontUseNativeDialog*/);

    filename = qtFilename.toStdString();

    return (!qtFilename.isEmpty());
}

void updateGUI()
{
    updateToolsGUI();
}
