#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void update_status(const QString& text);

private slots:
    void on_startStop_clicked();
    void update_statusBarUI(const QString& text);

private:
    void copy_file(const QString& path);

    Ui::MainWindow *ui;
    QStatusBar *statusBar;
    QFileSystemWatcher *watcher;
    QString destinationDir;
};
#endif // MAINWINDOW_H
