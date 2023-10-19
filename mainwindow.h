#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>

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

    void on_MountPathButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList *SessionPlugins;
    QJsonArray InstalledPlugins;
    QString *mountfolder;

    void readJson();
    void writeJson(QJsonArray arr);
    void updateMounts(QJsonArray arr);
};
#endif // MAINWINDOW_H
