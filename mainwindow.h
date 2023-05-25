
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "luxhandler.h"
#include "logger.h"
#include <QMainWindow>
#include <QTimer>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void capture();
    void capture_timer();
    void save_capture(std::string file);
    void com_Lux();
    void updateLuxmeter(QByteArray data);
    void change_Com();


public slots:
    void slotTimer();

private:
    Ui::MainWindow *ui;
    Logger *l;
    QTimer timer;
    std::string filepath;
    int duration;
    int intervalle;
    LuxHandler *serialLux;
    QThread *luxmeter_Thread;

};

#endif // MAINWINDOW_H
