#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUrl>
#include <QDesktopServices>
#include <QProcess>
#include <QFile>
#include <QDate>

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
    QString defHost;
    QString defPort;
    bool reachedServer;
    void log(QString, QString);
    QFile *LogFile;
    QTextStream *LogStream;
    bool loopActEd;

private slots:
    void on_pushB_fixError_clicked();
    void on_pushB_startNetAnal_clicked();
    void on_pushB_exit_clicked();

    void processNasState(QAbstractSocket::SocketState);
    void processNasError(QAbstractSocket::SocketError);

    void processLink(QUrl);

    void runLocalServer();
    void manageInQuery();
    void readIntSocket();
    void readExtSocket();
    void manageDisco();
    void connectedToExt();

    void setupLoopback(bool);
    void finishLoopbackAct(int);
    void finishLoopbackDeAct(int);


};

#endif // MAINWINDOW_H
