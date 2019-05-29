#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include<QtGui>
#include <QMessageBox>
#include<vector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_radio_stu_clicked();

    void on_radio_tea_clicked();
    void readdata();
    void discon();

    void on_course_clicked();

    void on_self_clicked();

    void on_showstu_clicked();

    void on_input_clicked();

    void on_showgrade_clicked();
    void show_t_com();

    void on_s_course_clicked();

    void on_s_show_own_course_clicked();

    void on_t_upgrade_clicked();

    void on_choose_course_clicked();

    void on_conf_clicked();

    void on_radiocom_clicked();

    void on_radioop_clicked();

    void on_confg_clicked();

    void on_logout_clicked();

    void on_conhand_clicked();

    void on_com2_clicked();

    void on_op2_clicked();
    void stem(int x);
    void stem2(int x);
    void on_t_downgrade_clicked();

    void on_t_upnum_clicked();

    void on_t_downgrade_2_clicked();

    void on_giveup_clicked();

    void on_s_upgrade_clicked();

    void on_s_downgrade_clicked();

    void on_s_upgpa_clicked();

    void on_s_downgpa_clicked();

    void on_show_all_grade_clicked();

    void on_logout_2_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int type = 0;
    int ctype = 0;
    int ctype2 = 0;
    QString number;
    QVector<QWidget*> widvec;
};

#endif // MAINWINDOW_H
