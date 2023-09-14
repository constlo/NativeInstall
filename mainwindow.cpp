#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QList>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QMap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gridLayout->setColumnMinimumWidth(0, 70);
    ui->gridLayout->setColumnMinimumWidth(1, 30);
    ui->gridLayout->setContentsMargins(5, 5, 5, 5);
    ui->gridLayout->setSizeConstraint(QLayout::SetMaximumSize);

    SessionPlugins = new QStringList();
    InstalledPlugins = new QStringList();

    QWidget *scrollContent = new QWidget;
    scrollContent->setLayout(ui->gridLayout);

    ui->scrollArea->setWidget(scrollContent);


}

MainWindow::~MainWindow()
{
    delete SessionPlugins;
    delete InstalledPlugins;
    delete ui;
}


void MainWindow::on_SelectButton_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                           "/home",
                                                           QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);
    if(!folderName.isEmpty())
    {
        //if we found a folder
        ui->folderLabel->setText(folderName);
    }
}


void MainWindow::on_ScanButton_clicked()
{
    if(!ui->folderLabel->text().isNull())
    {
        QProcess*  process = new QProcess(this);
        process->start("bash", QStringList() << "-c" << "ls " + ui->folderLabel->text() + " | grep .iso" );
        process->waitForFinished(-1);
        QString Res = process->readAllStandardOutput();
        QString Err = process->readAllStandardError();
        if(Err.isEmpty())
        {
            if(Res.isEmpty())
            {
                ui->ResultLabel->setText("No plugins Found!");
            }
            else
            {
                ui->ResultLabel->setText("Found the following isos: ");
                QStringList foundIsos = Res.split("\n");
                foundIsos.pop_back();
                foreach (QString str, foundIsos) {
                    if(!SessionPlugins->contains(str))
                    {
                        SessionPlugins->append(str);
                        qDebug() << str;
                        QLabel *PluginLabel = new QLabel(this, Qt::Widget);
                        PluginLabel->setObjectName(str);
                        PluginLabel->setText(str);
                        QPushButton *InstallButton = new QPushButton();
                        InstallButton->setObjectName(str);
                        InstallButton->setText("Install");
                        connect(InstallButton, &QPushButton::clicked, this, &MainWindow::onInstallClicked);
                        ui->gridLayout->addWidget(PluginLabel);
                        ui->gridLayout->addWidget(InstallButton);
                        ui->scrollArea->ensureWidgetVisible(InstallButton, 200, 200);
                        ui->scrollArea->setWidgetResizable(true);


                    }
                }

            }
        }else
        {
            ui->ResultLabel->setText("Error: " + Err);
        }
        delete process;
    }
}

void MainWindow::onInstallClicked()
{
    QObject *senderObj = sender();
    //if we have the sender object, retrieve it's name.
    if (senderObj)
    {
        QString name = senderObj->objectName();
        qDebug() << name;
        QProcess*  process = new QProcess(this);
        QString command = " mount -t udf " + ui->folderLabel->text() + "/" + name + " -o unhide ./mnt";
        qDebug() << command;
        process->start("/usr/bin/sudo", QStringList() << "-c" << command);
        process->waitForFinished(-1);
        QString Res = process->readAllStandardOutput();
        QString Err = process->readAllStandardError();
    }
}



