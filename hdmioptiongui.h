#ifndef HDMIOPTIONGUI_H
#define HDMIOPTIONGUI_H
#include <QDialog>
#include "tbsfunc.h"
#include "threadtbs.h"
#include "appinit.h"
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFile>
namespace Ui {
class Hdmioptiongui;
}

class Hdmioptiongui : public QDialog
{
    Q_OBJECT

public:
    explicit Hdmioptiongui(QWidget *parent = 0);
    ~Hdmioptiongui();
	int  getNetTuners(void);
	void initForm();
	int readBuffer(void);
	int readRF_TP_ARG(void);
	int readStream_ARG(void);
	int initStreamFrom(void);
	int read_IP_Port(void);
	int recvNetTStoLocalFile(void);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::Hdmioptiongui *ui;
    tbsfunc tbs;
    threadTbs *th;
    int width;
    int height;
    int width_s;
    int height_s;
    int fontsize;
	QMessageBox *qbox;
	QTableWidget *uitabW_PgmArg;
	QCheckBox *checkEN[CHECKNUM];
	QHBoxLayout *hLayoutEN[CHECKNUM];
	QWidget *widgetEN[CHECKNUM];
	int netnum;
	int pgnum;
private slots:
    void show_status(Msg* g);
    void on_sli_H_valueChanged(int value);
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
	void on_tw_Setting_currentChanged(int val);
	void on_btn_Read_clicked();
	void on_btn_Write_clicked();
	void showSelectedImage(QTreeWidgetItem * item, int column); //点击树节点事件
	void slot_check0_stateChanged(int idx);
	//void on_chec_EN_stateChanged(int idx);
	//void on_btn_Refresh_clicked();
};
#endif
