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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

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

    QWidget *installContent = new QWidget;
    installContent->setLayout(ui->gridLayout_2);
    ui->scrollArea_2->setWidget(installContent);

    //on start, we open the installed plugins .json object, if it exists. Create it if not.
    readJson();
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
                //The last element found is empty. Pop it from the stringlist.
                foundIsos.pop_back();
                foreach (QString str, foundIsos) {
                    if(!SessionPlugins->contains(str))
                    {
                        SessionPlugins->append(str);
                        qDebug() << str;
                        //we add the newly found elements into the scrollarea widget.
                        QLabel *PluginLabel = new QLabel(this, Qt::Widget);
                        PluginLabel->setObjectName(str);
                        PluginLabel->setText(str);
                        QPushButton *InstallButton = new QPushButton();
                        InstallButton->setObjectName(str);
                        InstallButton->setText("Mount");
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
        QString command = " mount -t udf " + ui->folderLabel->text() + "/" + name + " -o unhide " + ui->MountPathLabel->text();
        qDebug() << command;
        process->start("bash", QStringList() << "-c" << command);
        process->waitForFinished(-1);
        QString Res = process->readAllStandardOutput();
        QString Err = process->readAllStandardError();
        qDebug() << Res;
        qDebug() << Err;
        //If we have an error mounting the iso, report the error to the user.
        if(Err != "")
        {
            if(Err.contains("WARNING: source write-protected, mounted read-only.\n"))
            {
                ui->ErrorLabel->setText("Mounted " + name + " Successfully.\n Install the mounted software from your filesystem.");
            }
            else
            {
                ui->ErrorLabel->setText("Error: " + Err);
            }
        }
    }
}

void MainWindow::on_MountPathButton_clicked()
{
    QString MountPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
    "/home",    QFileDialog::ShowDirsOnly   |   QFileDialog::DontResolveSymlinks);
    if(!MountPath.isEmpty())
    {
        //if we found a folder
        ui->MountPathLabel->setText(MountPath);
    }
}

void MainWindow::readJson()
{
    //this function gets called on program start.
    //The opened file is a json array with each plugin containing
    //each plugin's name and installation date.
    QFile inFile("installedplugins.json");
    inFile.open(QIODevice::ReadOnly|QIODevice::Text);
    qDebug() << inFile.exists();
    if(!inFile.exists())
    {
        inFile.close();
        //create the file and write an empty JSON array into it, should it not exist.
        inFile.open(QIODevice::ReadWrite|QIODevice::Text);
        inFile.write("[]");
        inFile.close();
    }
    else
    {
        QByteArray data = inFile.readAll();
        inFile.close();

        QJsonParseError errorPtr;
        QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
        if (doc.isNull()) {
            qDebug() << "Parse failed";
            //if the file is not present, create it.
        }
        QJsonArray rootArray = doc.array();
        for(int i = 0; i < rootArray.size(); i++)
        {
            if(rootArray[i].isObject())
            {
                QJsonObject obj = rootArray[i].toObject();
                qDebug() << obj.value("installedFileName").toString();
                QLabel *installLabel = new QLabel(this, Qt::Widget);
                installLabel->setText(obj.value("installedFileName").toString());
                ui->gridLayout_2->addWidget(installLabel);
                ui->scrollArea_2->setWidgetResizable(true);
                ui->scrollArea->ensureWidgetVisible(installLabel, 200, 200);

            }
        }
        qDebug() << rootArray.size();
    }
}


