#ifndef QTUTILS_H
#define QTUTILS_H

#include <string>
#include <QWidget>

bool openFileSelectionDialog(std::string & filename,
                             const std::string & caption,
                             const std::string & filter ,
                             QWidget * parentWidget = nullptr);

bool openFileSaveDialog(std::string & filename,
                        const std::string & caption,
                        const std::string & filter,
                        QWidget * parentWidget = nullptr);

void updateGUI();

#endif // QTUTILS_H
