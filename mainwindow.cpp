#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFile"
#include "QFileDialog"
#include "QLabel"
#include "QPushButton"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    qDebug() << "123";
    resize(500, 500);
    setWindowTitle("File Compression");
    com=new Compression();
    // 创建并设置进度条
    progressBar = new QProgressBar(this);
    progressBar->resize(400, 30);
    progressBar->move(60, 20);
    progressBar->setRange(0, 100); // 设置范围（通常是0到100）
    progressBar->setValue(0);      // 设置初始值
    progressBar->setTextVisible(true); // 显示进度条的文本

    //放置按钮
    QPushButton *input = new QPushButton("选择", this);
    input->resize(80, 20);
    input->move(400, 100);
    QPushButton *output = new QPushButton("选择",this);
    output->resize(80, 20);
    output->move(400, 150);
    QPushButton *zipButton = new QPushButton("压缩", this);
    QPushButton *unzipButton = new QPushButton("解压", this);
    zipButton->move(40, 400);
    unzipButton->move(360, 400);


    //绑定

    //初始化压缩类指针
    connect(com,&Compression::error,this,&MainWindow::clear);
    //异常情况的信号接收
    connect(com,SIGNAL(mysignal(double)),this,SLOT(myslot(double)));
    connect(input, &QPushButton::clicked, this, MainWindow::on_pushButton_open_clicked);
    connect(output, &QPushButton::clicked, this, MainWindow::on_pushButton_close_clicked);
    connect(zipButton, &QPushButton::clicked, this, MainWindow::on_pushButton_compression_clicked);
    connect(unzipButton, &QPushButton::clicked, this, MainWindow::on_pushButton_decompression_clicked);

    QLabel *label1 = new QLabel("输入路径", this);
    label1->move(10, 95);
    QLabel *label2 = new QLabel("输出路径", this);
    label2->move(10, 145);

    lineEdit = new QLineEdit(this);
    lineEdit->resize(300, 20);
    lineEdit->move(80, 100);
    lineEdit->setReadOnly(true);

    lineEdit2 = new QLineEdit(this);
    lineEdit2->resize(300, 20);
    lineEdit2->move(80, 150);
    lineEdit2->setReadOnly(true);

    textEdit = new QTextEdit(this);
    textEdit->resize(400, 150);
    textEdit->move(50, 200);

    // 将LineEdit设置为只读
//    lineEdit->setEnabled(false);
//    lineEdit->setText("123");
//    QString path=QFileDialog::getOpenFileName(this,QString("选择文件"));


    // 示例：在定时器中模拟进度的增加
//    QTimer *timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, [=]() {
//        int currentValue = progressBar->value();
//        if (currentValue < progressBar->maximum()) {
//            progressBar->setValue(currentValue + 1);
//        } else {
//            timer->stop();
//        }
//    });
//    timer->start(50); // 50毫秒更新一次，模拟进度增加
}

MainWindow::~MainWindow()
{
    delete ui;
    delete com;
}




void MainWindow::clear()
{
    lineEdit->clear();
    lineEdit2->clear();
    path.clear();
    path2.clear();
}
//在文件打开失败的情况下清空所有

void MainWindow::myslot(double per)
{
    if(per>this->progressBar->value())
        this->progressBar->setValue(per);
}
//per是进度，信号是由ComPreesion类指针发出，这是槽函数

void MainWindow::on_pushButton_open_clicked()
{
    path=QFileDialog::getOpenFileName(this,QString("选择文件"));
    lineEdit->setText(path);
//    textEdit->setText("");
//    qDebug() << "123";
//    qDebug() << path;
//    name=path.mid(path.lastIndexOf("/"));
//    qDebug() << name;
}

void MainWindow::on_pushButton_close_clicked()
{
    path2 = QFileDialog::getExistingDirectory(this, "选择文件夹", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    lineEdit2->setText(path2);
}
//UI界面“选择”按钮的槽函数，更新path和lineEdit内容


void MainWindow::on_pushButton_compression_clicked()
{
    com->zip(path, path2);
    lineEdit->clear();
    lineEdit2->clear();
    this->path.clear();
    this->path2.clear();
    this->progressBar->setValue(0);
    textEdit->setText(com->getTip());
}
//UI界面“压缩”按钮的槽函数，将path作为参数传给压缩类的压缩函数，压缩完毕后，清除lineEdit，path，progressBar

void MainWindow::on_pushButton_decompression_clicked()
{
    com->unzip(path, path2);
    lineEdit->clear();
    lineEdit2->clear();
    this->path.clear();
    this->path2.clear();
    this->progressBar->setValue(0);
}


