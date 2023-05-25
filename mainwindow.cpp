
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <QTimer>
#include <chrono>
#include <QSerialPort>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    l = new Logger();

    luxmeter_Thread = new QThread();

    ui->lcdLuxmeter->setDigitCount(6);

    // creating a lux_thread object and opening connection with a port
    serialLux = new LuxHandler;

    serialLux->openConnection();

    // connecting the signal of getting a newdata with the luxmeter lcd update function
    connect(serialLux,&LuxHandler::gotNewData,this, &MainWindow::updateLuxmeter);
    // moving the luxmeter update to another thread so it will run without creating active waiting
    serialLux->moveToThread(luxmeter_Thread);
    serialLux->setCombo(ui->comBox);

    // starting the thread for the luxmeter
    luxmeter_Thread->start();

    // Sync. the action with button in charge of the one click capture for the luxmeter values
    connect(ui->inst_capture,&QPushButton::clicked,this, &MainWindow::capture);
    // Sync. tha action with button in charge of the capture in time of the luxmeter values
    connect(ui->timerButton,&QPushButton::clicked,this, &MainWindow::capture_timer);

    // Sync. the action with the button in charge of changing the port
    connect(ui->comButton,&QPushButton::clicked,this,&MainWindow::change_Com);

    // Sync. the timeout action of the timer with the slot function
    QObject::connect(&timer, SIGNAL(timeout()),this, SLOT(slotTimer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief UserPanel::change_Com
 * Function to change the com port of the luxmeter
 */
void MainWindow::change_Com()
{
    // getting the selected port and opening a connection with it
    QString com = ui->comBox->currentText();
    this->serialLux->closeConnection();
    this->serialLux->setCom(com);
    this->serialLux->openConnection();
}

/**
 * @brief UserPanel::updateLuxmeter
 * Function to update the display of the luxmeter
 * @param data : the data to display
 */
void MainWindow::updateLuxmeter(QByteArray data)
{
    float myInt = data.toInt();
    ui->lcdLuxmeter->display(myInt);
}

/**
 * @brief LuxMeter::slotTimer
 * Slot function to execute by the timer when he times out
 */
void MainWindow::slotTimer()
{
    ///
    /// The timer act not as you may think, here a quick explaination :
    /// after every period of time ( = one intervalle) the timer will execute an action
    /// we changed this action in the 'main' function of the class
    /// now the timer will run as long as the program is running
    /// so to stop the timer after a certain amount of time we decrement the time left by the intervalle time
    /// when the duration reach 0 it will be the last iteration of the timer
    ///
    this->duration -= this->intervalle;
    if(this->duration < 0)
    {
        // We stop the timer and log it
        this->l->write_log("luxmeter mesurement cycle ended");
        this->timer.stop();
    }
    else
    {
        // we save the luxmeter value
        save_capture(this->filepath);
    }

}

/**
 * @brief LuxMeter::capture
 * Function called when instant capture button is clicked
 */
void MainWindow::capture()
{
    // creating the filepath to save the value
    this->filepath = "";
    // getting the time and date to name the file
    std::string time = this->l->getCurrentTime();
    this->filepath = "./capture/" + time + ".csv";
    l->write_log("capturing luxmeter values");
    // saving the value using the filepath created
    save_capture(this->filepath);

}

/**
 * @brief LuxMeter::capture_timer
 * Function called when the timed capture button is clicked
 */
void MainWindow::capture_timer()
{
    // getting and setting the values of the intervalle and the duration of the capture
    this->intervalle = ui->inter_capture_2->value();
    this->duration = ui->inter_capture->value();

    // creating the filepath to save the values
    this->filepath = "";
    // getting the time and date to name the file
    std::string time = this->l->getCurrentTime();
    this->filepath = "./capture/" + time + ".csv";
    l->write_log("capturing luxmeter values for " + std::to_string(this->duration) + " secondes");

    // setting the timer with the intervalle * 1000 (because we want the intervalle in millisecond
    timer.setInterval((this->intervalle)*1000);
    // if this setting is true, the timer will act only once after the intervalle
    timer.setSingleShot(false);
    // Starting the timer
    timer.start();
}

/**
 * @brief LuxMeter::save_capture
 * This function will save in a given file the actual value of the luxmeter
 * @param f : the file we will save the value in
 */
void MainWindow::save_capture(std::string f)
{
    std::ofstream file;
    // getting the current time and date
    std::string time = this->l->getCurrentTime();
    // getting the value of the luxmeter
    int val = ui->lcdLuxmeter->value();
    std::string value = std::to_string(val);

    // we check if the file already exist
    struct stat buffer;
    if(stat (f.c_str(),&buffer) == 0)
    {
        // if it exist we open it with the appened condition
        file.open(f,std::ios::app);
        //if it didn't open we log an error
        if(!file.is_open())
        {
            this->l->write_error("Problem encountered : capture file didn't open properly");
        }
        // if it did open we write the value with the date/time in the file
        else
        {
            file << time << ',' << value << std::endl;
        }

    }
    //if the file doesn't exist we will create it
    else
    {
        // logging the creation of the file
        this->l->write_log(f + " was created");
        file.open(f);
        if(!file.is_open())
        {
            this->l->write_error("Problem encountered : capture file wasn't created correctly");
        }
        file << time << ',' << value << std::endl;
    }

}
