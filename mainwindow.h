#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interface.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changeItemsname();

    void load();
    void save();
    void exit();
    void del();
    void reset();
    void doc();



protected:
    void changeEvent(QEvent *e);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
