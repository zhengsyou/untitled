#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cznetworkaccessmanager.h"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThreadPool>

#include <functional>

class HelloWorldTask : public QRunnable
{
public:
    HelloWorldTask(const std::function<void(std::vector<void *>)> &func, const std::vector<void*> &vecParams)
        :m_func(func), m_vecParams(vecParams)
    {
    }

    void run()
    {
        m_func(m_vecParams);
    }
private:
    std::function<void(std::vector<void *>)> m_func;
    std::vector<void*> m_vecParams;
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    std::vector<void*> vecParams;
    QThreadPool::globalInstance()->start(new HelloWorldTask([](const std::vector<void*> &vecParams){
        qDebug() << vecParams.size();
    }, vecParams));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::queryKey(const QString &strText)
{
    QString strKey = "";
    if(QStringLiteral("签名") == strText )
    {
        strKey = "signature";
    }
    else if(QStringLiteral("文件名") == strText)
    {
        strKey = "filename";
    }
    else if(QStringLiteral("路径") == strText )
    {
        strKey = "filepath";
    }
    else if(QStringLiteral("公司信息") == strText )
    {
        strKey = "company";
    }
    else if(QStringLiteral("文件描述") == strText )
    {
        strKey = "description";
    }
    else if(QStringLiteral("PDB路径") == strText )
    {
        strKey = "pdbpath";
    }
    else if(QStringLiteral("图标") == strText )
    {
        strKey = "iconcrc";
    }
    return strKey;
}

QString MainWindow::queryData(const QString &strKey, const QString &strData)
{
    CZNetworkAccessManager manager;
    QString strUrl = QString("http://hunterserver.com/api/query.php?t=%1&v=%2")
            .arg(strKey)
            .arg(QString(QUrl::toPercentEncoding(strData)));
    QString strRetData;
    QNetworkRequest request(strUrl);
    QNetworkReply *reply = manager.get(request);
    if(QNetworkReply::NoError == reply->error())
    {
        strRetData = reply->readAll();
    }

    delete reply;
    return strRetData;
}

void MainWindow::ShowData(QJsonArray ary)
{
    ui->tableWidget->setRowCount(ary.size());
    for(int i = 0; i < ary.size(); i++)
    {
        QJsonValue value = ary[i];
        QJsonObject obj = value.toObject();
        QJsonValue vaT = obj["_source"];
        obj = vaT.toObject();
        QString strMD5 = QUrl::fromPercentEncoding(obj["md5"].toString().toLocal8Bit());
        QString strFilePath = QUrl::fromPercentEncoding(obj["filepath"].toString().toLocal8Bit());
        QString strLevel = QUrl::fromPercentEncoding(obj["level"].toString().toLocal8Bit());
        QString strPDB = QUrl::fromPercentEncoding(obj["pdbpath"].toString().toLocal8Bit());

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(strMD5));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(strLevel));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(strFilePath));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(strPDB));
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->clearContents();

    QString strKey = queryKey(ui->comboBox->currentText());
    if(strKey.isEmpty())
    {
        return ;
    }

    QString strRetData = queryData(strKey, ui->lineEdit->text());
    if(strRetData.isEmpty())
    {
        return ;
    }

    QJsonDocument json = QJsonDocument::fromJson(QUrl::toPercentEncoding(strRetData, "[{]},\"_:"));
    if(json.isEmpty())
    {
        return ;
    }

    ShowData(json.array());
}
