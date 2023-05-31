#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStatusBar>
#include <QDir>
#include <QFileSystemWatcher>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statusBar(new QStatusBar)
    , watcher(new QFileSystemWatcher)
{
    // Initialize the UI
    ui->setupUi(this);

    // Initialize textbox values
    ui->watchDir->setText(QDir::toNativeSeparators("C:/ProgramData/Siemens/Numaris/SimMeasData/NORDIC"));
    ui->copyDir->setText(QDir::toNativeSeparators("Y:/"));

    // Create a status bar
    ui->statusLayout->addWidget(statusBar);
    statusBar->showMessage("Idle");

    // Connect the watcher to the directoryChanged event
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::copy_file);

    // Connect the update_status signal to the update_statusBarUI method
    connect(this, &MainWindow::update_status, this, &MainWindow::update_statusBarUI);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_statusBarUI(const QString& text) {
    statusBar->showMessage(text);
}

void MainWindow::copy_file(const QString& path) {
    // Get the list of files in the source directory
    QDir directory(path);
    QStringList files = directory.entryList(QStringList() << "*.dat", QDir::Files);

    // Move each file to the destination directory
    for (const QString& file : files) {
        QString sourcePath = directory.absoluteFilePath(file);
        QString destinationPath = QDir(destinationDir).absoluteFilePath(file);

        // Perform the file move (copy and delete)
        if (QFile::copy(sourcePath, destinationPath)) {
            emit update_status("Copied " + file);
            QFile::remove(sourcePath); // Delete the original file
        } else {
            qDebug() << "Failed to move file:" << sourcePath;
        }
    }
}

void MainWindow::on_startStop_clicked()
{
    // First disable the button
    ui->startStop->setEnabled(false);

    // Start or stop transfer based on button state
    if (ui->startStop->text() == "Start") {
        // Disable the textboxes
        ui->watchDir->setEnabled(false);
        ui->copyDir->setEnabled(false);

        // Update the destinationDir
        destinationDir = ui->copyDir->text();

        // Set the status
        emit update_status("Watching for files in " + ui->watchDir->text());

        // Start watcher on path
        watcher->addPath(ui->watchDir->text());

        // Change button text to Stop
        ui->startStop->setText("Stop");
    }
    else if(ui->startStop->text() == "Stop") {
        // Re-enable the textboxes
        ui->watchDir->setEnabled(true);
        ui->copyDir->setEnabled(true);

        // Stop the watcher
        watcher->removePaths(watcher->directories());

        // Set the status
        emit update_status("Idle");

        // Change button text to Start
        ui->startStop->setText("Start");
    }

    // Now reenable the button
    ui->startStop->setEnabled(true);
}

