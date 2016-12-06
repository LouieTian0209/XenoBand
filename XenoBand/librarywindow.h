#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

#include <QWidget>

namespace Ui {
class LibraryWindow;
}

class LibraryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LibraryWindow(QWidget *parent = 0);
    ~LibraryWindow();

private slots:
    void on_createButton_clicked();

private:
    Ui::LibraryWindow *ui;
};

#endif // LIBRARYWINDOW_H
