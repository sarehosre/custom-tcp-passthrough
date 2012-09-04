#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer *Server;
    QTcpSocket *externalSocket;
    QTcpSocket *internalSocket;

private slots:
    void manageInQuery();
    void readIntSocket();
    void readExtSocket();
    void manageDisco();
    void connectedToExt();

};

#endif // MAINWINDOW_H
