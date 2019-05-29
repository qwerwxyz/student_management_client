#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

int usernumber;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setTabEnabled(4, false);
    ui->tabWidget->setTabEnabled(5, false);
    ui->logout->hide();
    socket = new QTcpSocket();
    QObject::connect(socket,&QTcpSocket::readyRead,this,&MainWindow::readdata);
    QObject::connect(socket,&QTcpSocket::disconnected,this,&MainWindow::discon);
}

MainWindow::~MainWindow()
{
    on_logout_clicked();
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    int Port = 8765;
    QRect qq = ui->verticalLayout->geometry();
    socket->abort();//取消已有连接
    socket->connectToHost("127.0.0.1",Port);//连接服务器
    try{
        if(!socket->waitForConnected(1000))
        {

            throw 2;
            return ;
        }
        if(type == 0) throw 3;
        if (ui->textEdit->text() == tr("")) throw 4;
        QString str = "log\n";
        socket->write(str.toLatin1());
        socket->write((QString::number(type) + "\n").toLatin1());
        socket->write((ui->textEdit->text() + "\n").toLatin1());
        socket->write((ui->textEdit_3->text()+"\n").toLatin1());
        socket->flush();

    }
    catch(int a){
        if(a == 2){
            QMessageBox::warning(this, tr("警告"), tr("无法连接"));
        }
        else if (a == 3) {
            QMessageBox::warning(this, tr("警告"), tr("请选择学生或教师"));
        }
        else if (a == 4) {
            QMessageBox::warning(this, "警告", "请输入学号或工号");
        }
    }

}

void MainWindow::readdata() {
    QByteArray buffer = socket->readLine();
    if (buffer == "fail\n") {
        QMessageBox::warning(this, "警告", "登陆失败");
    }
    else if(buffer=="mistake\n"){
        QMessageBox::warning(this, "警告", "密码错误");
    }
    else if(buffer == "success\n"){
        buffer = socket->readLine();
        if(buffer == "tea\n"){
            type = 2;
            ui->tabWidget->setTabEnabled(0, false);
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setTabEnabled(3, true);
            ui->tabWidget->setTabEnabled(4, true);
            ui->tabWidget->setCurrentIndex(1);
            number = (QString::fromLatin1(socket->readLine()));
        }
        else if(buffer == "stu\n"){
            type = 1;
            ui->tabWidget->setTabEnabled(0, false);
            ui->tabWidget->setTabEnabled(2, true);
            ui->tabWidget->setTabEnabled(5, true);
            ui->tabWidget->setCurrentIndex(2);
            number = (QString::fromLatin1(socket->readLine()));
        }
        ui->logout->show();
    }
    else if (buffer == "data\n") {

        buffer = socket->readAll();
        ui->data->setText(QString::fromUtf8(buffer));
    }
    else if (buffer == "returnstu\n"){
        QString str1;
        bool ok;
        int n = QString::fromLatin1(socket->readLine()).toInt();
        for (int i = 0; i < widvec.size(); i++) delete widvec[i];
        widvec.clear();
        for (int i = 0; i < n; i++) {
            QLineEdit *le = new QLineEdit(ui->inwid);
            widvec << le;
            le->show();
            le->resize(300, 30);
            le->move(0, 50 * i);
            le->setObjectName(QString::number(i));
            buffer = socket->readLine();
            le->setPlaceholderText(QString::fromUtf8(buffer));

        }


    }
    else if (buffer == "tip\n") {
        QMessageBox::warning(this, tr(""), QString::fromUtf8(socket->readLine()));
    }
    else if (buffer == "numerror\n") {
        QMessageBox::warning(this, tr("警告"), tr("课程序号错误"));
    }
    else if (buffer == "changeerror\n") {
        QMessageBox::warning(this, tr("警告"), tr("不可更改"));
    }
    else if (buffer == "gradeerror\n") {
        QMessageBox::warning(this, tr("警告"), tr("输入成绩错误"));
    }
    else if (buffer == "handsuccess\n") {
        QMessageBox::warning(this, tr("成功"), tr("提交成功"));
    }
    else if (buffer == "handfail\n") {
        QMessageBox::warning(this, tr("警告"), tr("提交失败"));
    }
    else if (buffer == "usernameerror\n") {
        QMessageBox::warning(this, tr("警告"), tr("没有该用户"));
    }
}

void MainWindow::discon() {
    ui->tabWidget->setTabEnabled(0, true);
    for (int i = 1; i <= 5; i++)
        ui->tabWidget->setTabEnabled(i, false);
    ui->data->clear();
}

void MainWindow::on_radio_stu_clicked()
{
    type = 1;
}

void MainWindow::on_radio_tea_clicked()
{
    type = 2;
}

void MainWindow::on_course_clicked()
{
    socket->write(tr("course\n").toLatin1());
    socket->flush();
}

void MainWindow::on_self_clicked()
{
    socket->write(tr("self\n").toLatin1());
    socket->write((number).toLatin1());
    socket->flush();
}

void MainWindow::on_showstu_clicked()
{
    socket->write(tr("showstu\n").toLatin1());
    socket->write((number).toLatin1());
    socket->flush();
}

void MainWindow::on_input_clicked()
{
    char p = 0, q = 0;
    on_self_clicked();//显示自身任教的课程
    ui->tabWidget->setCurrentIndex(3);
    ui->tabWidget->setTabEnabled(3, true);
}

void MainWindow::show_t_com(){
    ui->teag->setVisible(true);
}

void MainWindow::on_showgrade_clicked()
{
    on_self_clicked();//显示自身任教的课程
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::on_s_course_clicked()
{
    socket->write(tr("course\n").toLatin1());
    socket->flush();
}

void MainWindow::on_s_show_own_course_clicked()
{
    socket->write(tr("show_own_course\n").toLatin1());
    socket->write((number).toLatin1());
    socket->flush();
}

void MainWindow::stem(int x) {
    try {
        if (ctype2 == 0) throw 1;
        bool ok;
        ui->line2->text().toInt(&ok);
        if (!ok) throw 2;
        socket->write(tr("asksort\n").toLatin1());
        socket->write(number.toLatin1());
        socket->write((QString::number(ctype2) + "\n").toLatin1());
        socket->write((ui->line2->text() + "\n").toLatin1());
        socket->write((QString::number(x) + "\n").toLatin1());
        socket->flush();
    }
    catch (int x) {
        if (x == 1) QMessageBox::warning(this, tr("警告"), tr("请选择课程"));
        if (x == 2) QMessageBox::warning(this, tr("警告"), tr("请输入正确课程编号"));
    }
}

void MainWindow::on_t_upgrade_clicked()
{
    stem(3);
}

void MainWindow::on_choose_course_clicked()
{
    on_course_clicked();
    bool ok;
    int i = QInputDialog::getInt(this, tr("请输入选修课编号"),
                                     tr("编号:"), 0, 0, 100, 1, &ok);
    if (!ok) QMessageBox::warning(this, tr("警告"), tr("请输入正确编号"));
    else {
        socket->write(tr("choose\n").toLatin1());
        socket->write(number.toLatin1());
        socket->write((QString::number(i) + "\n").toLatin1());
        socket->flush();
    }
}

void MainWindow::on_conf_clicked()
{
    try {
        if (ctype == 0) throw 1;
        bool ok;
        ui->lineEdit->text().toInt(&ok);
        if (!ok) throw 2;
        socket->write(tr("askstu\n").toLatin1());
        socket->write(number.toLatin1());
        socket->write((QString::number(ctype) + "\n").toLatin1());
        socket->write((ui->lineEdit->text() + "\n").toLatin1());
        socket->flush();
    }
    catch (int x) {
        if (x == 1) QMessageBox::warning(this, tr("警告"), tr("请选择课程"));
        if (x == 2) QMessageBox::warning(this, tr("警告"), tr("请输入正确课程编号"));
    }
}

void MainWindow::on_radiocom_clicked()
{
    ctype = 1;
}

void MainWindow::on_radioop_clicked()
{
    ctype = 2;
}

void MainWindow::on_confg_clicked()
{
    try {
        if (ctype == 0) throw 1;
        bool ok;
        ui->lineEdit->text().toInt(&ok);
        if (ui->lineEdit->text() != "" && !ok) throw 2;
        socket->write(tr("inputgrade\n").toLatin1());
        socket->write(number.toLatin1());
        socket->write((QString::number(ctype) + "\n").toLatin1());
        socket->write((ui->lineEdit->text() + "\n").toLatin1());
        socket->write((QString::number(widvec.size()) + "\n").toLatin1());
        for (int i = 0; i < widvec.size(); i++) {
            QLineEdit *le = ui->centralWidget->findChild<QLineEdit*>(QString::number(i));
            socket->write((le->text()+"\n").toLatin1());
        }
        socket->flush();
    }
    catch (int x) {
        if (x == 1) QMessageBox::warning(this, tr("警告"), tr("请选择课程"));
        if (x == 2) QMessageBox::warning(this, tr("警告"), tr("请输入正确课程编号"));
    }
}

void MainWindow::on_logout_clicked()
{
    discon();
    socket->write(tr("logout\n").toLatin1());
    socket->write((QString::number(type) + "\n").toLatin1());
    socket->write(number.toLatin1());
    socket->flush();
    ui->logout->hide();
}

void MainWindow::on_conhand_clicked()
{
    try {
        if (ctype == 0) throw 1;
        bool ok;
        ui->lineEdit->text().toInt(&ok);
        if (ui->lineEdit->text() != "" && !ok) throw 2;
        socket->write(tr("handgrade\n").toLatin1());
        socket->write(number.toLatin1());
        socket->write((QString::number(ctype) + "\n").toLatin1());
        socket->write((ui->lineEdit->text() + "\n").toLatin1());
        socket->flush();
    }
    catch (int x) {
        if (x == 1) QMessageBox::warning(this, tr("警告"), tr("请选择课程"));
        if (x == 2) QMessageBox::warning(this, tr("警告"), tr("请输入正确课程编号"));
    }
}

void MainWindow::on_com2_clicked()
{
    ctype2 = 1;
}

void MainWindow::on_op2_clicked()
{
    ctype2 = 2;
}

void MainWindow::on_t_downgrade_clicked()
{
    stem(4);
}

void MainWindow::on_t_upnum_clicked()
{
    stem(1);
}

void MainWindow::on_t_downgrade_2_clicked()
{
    stem(2);
}

void MainWindow::on_giveup_clicked()
{
    on_course_clicked();
    bool ok;
    int i = QInputDialog::getInt(this, tr("请输入选修课编号"),
                                     tr("编号:"), 0, 0, 100, 1, &ok);
    if (!ok) QMessageBox::warning(this, tr("警告"), tr("请输入正确编号"));
    else {
        socket->write(tr("withdraw\n").toLatin1());
        socket->write(number.toLatin1());
        socket->write((QString::number(i) + "\n").toLatin1());
        socket->flush();
    }
}

void MainWindow::stem2(int a) {
    socket->write(tr("showallgrade\n").toLatin1());
    socket->write(number.toLatin1());
    socket->write((QString::number(a) + "\n").toLatin1());
    socket->flush();
}
void MainWindow::on_s_upgrade_clicked()
{
    stem2(2);
}

void MainWindow::on_s_downgrade_clicked()
{
    stem2(1);
}

void MainWindow::on_s_upgpa_clicked()
{
    stem2(4);
}

void MainWindow::on_s_downgpa_clicked()
{
    stem2(3);
}

void MainWindow::on_show_all_grade_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

void MainWindow::on_logout_2_clicked()
{
    socket->write(tr("renew\n").toLatin1());
    socket->flush();
}
