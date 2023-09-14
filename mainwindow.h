#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ScanButton_clicked();

    void on_SelectButton_clicked();
    void onInstallClicked();

private:
    Ui::MainWindow *ui;
    QStringList *SessionPlugins;
    QStringList *InstalledPlugins;
};
#endif // MAINWINDOW_H
