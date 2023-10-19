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
#include <QDateTime>

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
    InstalledPlugins = QJsonArray();

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
    //upon closing the program, write contents of the installedplugins json to the file.
    writeJson(InstalledPlugins);
    delete ui;
}


void MainWindow::on_SelectButton_clicked()
{
    //this slot opens up the folder of the downloaded .iso files and stores the folder path to ui->folderLabel.
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
    //this slot scans the folder using ls | grep .iso command on linux, and adds them to the scroll area with an install button.
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
            //this warning indicates that the mount was succesful.
            if(Err.contains("WARNING: source write-protected, mounted read-only.\n"))
            {
                ui->ErrorLabel->setText("Mounted " + name + " Successfully.\n Install the mounted software from your filesystem.");
                //now we push the new installed iso to the qjson array, as well as the date of the mounting.
                QJsonObject object = QJsonObject();
                QDateTime dateTime = QDateTime();
                object.insert("installedFileName", name);
                object.insert("dateOfInstall", dateTime.date().toString());
                InstalledPlugins.append(object);
                qDebug() << InstalledPlugins;
                updateMounts(InstalledPlugins);
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
    //this slot is used to retrieve the mount path.
    QString MountPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
    "/home",    QFileDialog::ShowDirsOnly   |   QFileDialog::DontResolveSymlinks);
    if(!MountPath.isEmpty())
    {
        //if we found a folder
        ui->MountPathLabel->setText(MountPath);
    }
}



void MainWindow::updateMounts(QJsonArray arr)
{
    //this method gets called in two situations:
    //the user opens the program, in which the installedplugins.json is either generated or read.
    //the user mounts an iso using the UI.
    //this method updates the UI's installed plugins section with new info on mounted plugins.

    //delete the scrollarea's widgets, should there be any.
    //first get the gridlayout's size.
    int size = ui->gridLayout_2->count();
    qDebug() << size;
    if(size > 0)
    {
        for(int i = 0; i < size; i++)
        {
            delete ui->gridLayout_2->itemAt(0)->widget();
        }
    }

    for(int i = 0; i < arr.size(); i++)
    {
        if(arr[i].isObject())
        {
            QJsonObject obj = arr[i].toObject();
            qDebug() << obj.value("installedFileName").toString();
            QLabel *installLabel = new QLabel(this, Qt::Widget);
            installLabel->setText(obj.value("installedFileName").toString());
            ui->gridLayout_2->addWidget(installLabel);
            ui->scrollArea_2->setWidgetResizable(true);
            ui->scrollArea->ensureWidgetVisible(installLabel, 200, 200);
        }
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
        InstalledPlugins = doc.array();
        updateMounts(InstalledPlugins);
    }
}


void MainWindow::writeJson(QJsonArray arr)
{
    //this method saves the installed plugins to installedplugins.json upon closing the program.
    QJsonDocument jsonDoc(InstalledPlugins);
    QByteArray byteArray = jsonDoc.toJson();
    qDebug()<< arr;
    QFile inFile("installedplugins.json");
    inFile.open(QIODevice::ReadWrite|QIODevice::Text);
    inFile.write(byteArray);
    inFile.close();
}
