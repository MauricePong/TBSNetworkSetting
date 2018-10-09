#pragma execution_character_set("utf-8")

#include "hdmioptiongui.h"
#include "ui_hdmioptiongui.h"
#include "iconhelper.h"
#define  ITMEL(n)   ITMEL##n


Hdmioptiongui::Hdmioptiongui(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Hdmioptiongui)
{
	ui->setupUi(this);
	this->initForm();
	//getNetTuners();
	//mainParseTS();
	/********************************************
	int iProgramIndex = 0;
	qDebug("out put all the program information ");
	qDebug("ProgramId\tPMT_PID\tPCR_PID\tProgramName\tVideoPID\tAudioPID1\tAudioPID2");
	for (iProgramIndex = 0; iProgramIndex < iProgramCount_g; iProgramIndex++)
	{
		qDebug("%5d\t%5d\t%5d\t%-25s\t%5d\t%5d\t%5d",
			astProgramInfo_g[iProgramIndex].uiProgramNumber,
			astProgramInfo_g[iProgramIndex].uiPMT_PID,
			astProgramInfo_g[iProgramIndex].uiPCR_PID,
			astProgramInfo_g[iProgramIndex].aucProgramNanme,
			astProgramInfo_g[iProgramIndex].uiVideoPID,
			astProgramInfo_g[iProgramIndex].auiAudioPID[0],
			astProgramInfo_g[iProgramIndex].auiAudioPID[1]);
	}
	**************************************************/
}

Hdmioptiongui::~Hdmioptiongui()
{
	swflg = 1;
	tbs.udpClose(udpfd);
	QFile rmfile(".tbsts");
	if (rmfile.exists())
	{
		rmfile.remove();
	}
	delete ui;
}

int Hdmioptiongui::readBuffer()
{
	//if (0 == twArg_flg) {
		//readRF_TP_ARG();
	//}
	//else if (1 == twArg_flg) {
		//readStream_ARG();
	//}


	return 0;
}

int Hdmioptiongui::readRF_TP_ARG(void)
{
	for (int i = 0; i < 8; i++) {
		rftp[i].upfreq = 0;
		rftp[i].downfreq = 0;
		rftp[i].switchfreq = 0;
		rftp[i].tp_if = 0;
		rftp[i].tp_symbolrate = 0;
		rftp[i].tp_hv = 0;
		rftp[i].diseqc_data = 0;
	}
	for (int i = 0; i < 8; i++) {

	}
	return 0;
}



int Hdmioptiongui::readStream_ARG(void)
{
	for (int i = 0; i < 8; i++) {
		streamarg[i].ip_addr = QString("");
		streamarg[i].udp_port = 0;
		streamarg[i].rtp_gate = 0;
		streamarg[i].all_ts_through = 0;
		streamarg[i].vpid = 0;
		streamarg[i].apid = 0;
		streamarg[i].pcr = 0;
		streamarg[i].pmt = 0;
	}
	/**
	u8 tmp[24] = { 0 };
	for (int i = 0; i < 8; i++) {
		tbs.target_ext_memory_rd_wt(READ, 0x0300 + 0x08 + 0x18 * i, tmp, 24);
		streamarg[i].rtp_gate = ((tmp[0] >> 7) & 0x01);
		streamarg[i].all_ts_through = ((tmp[0] >> 6) & 0x01);
		streamarg[i].rtp_type = ((tmp[0] >> 5) & 0x01);
		Myswap(tmp[2], tmp[3]);
		streamarg[i].udp_port = *(u16*)&tmp[2];
		streamarg[i].ip_addr = QString("%1.%2.%3.%4").arg(tmp[4])\
			.arg(tmp[5]).arg(tmp[6]).arg(tmp[7]);
		Myswap(tmp[8], tmp[9]);
		Myswap(tmp[10], tmp[11]);
		Myswap(tmp[12], tmp[13]);
		Myswap(tmp[14], tmp[15]);
		streamarg[i].vpid = *(u16 *)&tmp[8];
		streamarg[i].apid = *(u16 *)&tmp[10];
		streamarg[i].pcr = *(u16 *)&tmp[12];
		streamarg[i].pmt = *(u16 *)&tmp[14];
	}
	*/
	//STREAM_INIT_FORM();
	return 0;
}

int Hdmioptiongui::initStreamFrom(void)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < CHECKNUM; i++) {
		if (NULL != checkEN[i]) {
			if (0 == i) {
				disconnect(checkEN[0], SIGNAL(stateChanged(int)), this, SLOT(slot_check0_stateChanged(int)));
			}
			delete checkEN[i];
			checkEN[i] = NULL;
		}

		if (NULL != hLayoutEN[i]) {
			delete hLayoutEN[i];
			hLayoutEN[i] = NULL;
		}
		if (NULL != widgetEN[i]) {
			delete widgetEN[i];
			widgetEN[i] = NULL;
		}
	}
	uitabW_PgmArg->clear();
	uitabW_PgmArg->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//uitabW_PgmArg->setColumnWidth(0,10);
	//uitabW_PgmArg->setColumnWidth(6,10);

	//uitabW_PgmArg->horizontalHeader()->setStretchLastSection(true);
	//uitabW_PgmArg->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	uitabW_PgmArg->setWindowTitle("Streaming");
	uitabW_PgmArg->setRowCount(pgnum + 1);
	uitabW_PgmArg->setColumnCount(7);
	for (i = 0; i < (pgnum + 1); i++) {
		if (i < 7) {
			uitabW_PgmArg->verticalHeader()->setVisible(false);
		}
		uitabW_PgmArg->horizontalHeader()->setVisible(false);
	}
	QString qstlab[7] = { QString(""), QString("Program id"),QString("Program name"),
	QString("PID(V_PID,A_PID,PCR,PMT)"),QString("Unicast/Multicast IP&&Port"),
	QString("Net protocol"),QString("Streaming status)") };
	for (i = 0; i < 7; i++) {
		uitabW_PgmArg->setItem(0, i, new QTableWidgetItem(qstlab[i]));
		uitabW_PgmArg->item(0, i)->setTextAlignment(Qt::AlignCenter);
		uitabW_PgmArg->item(0, i)->setFlags(Qt::ItemIsEnabled);
		if (i == 0) {
			checkEN[i] = new QCheckBox();
			checkEN[i]->setCheckState((1) ? (Qt::Checked) : (Qt::Unchecked));
			checkEN[i]->setEnabled(true);
			checkEN[i]->setText(QString(""));
			hLayoutEN[i] = new QHBoxLayout();
			widgetEN[i] = new QWidget(uitabW_PgmArg);
			hLayoutEN[i]->addWidget(checkEN[i]);
			hLayoutEN[i]->setMargin(0); // 必须添加, 否则CheckBox不能正常显示
			hLayoutEN[i]->setAlignment(checkEN[i], Qt::AlignCenter);
			widgetEN[i]->setLayout(hLayoutEN[i]);
			uitabW_PgmArg->setCellWidget(j, i, widgetEN[i]);
			j++;
		}
	}
	for (int i = 1; i < (pgnum + 1); i++) {
		checkEN[i] = new QCheckBox();
		checkEN[i]->setCheckState((1) ? (Qt::Checked) : (Qt::Unchecked));
		checkEN[i]->setEnabled(true);
		checkEN[i]->setText(QString(""));
		hLayoutEN[i] = new QHBoxLayout();
		widgetEN[i] = new QWidget(uitabW_PgmArg);
		hLayoutEN[i]->addWidget(checkEN[i]);
		hLayoutEN[i]->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
		hLayoutEN[i]->setAlignment(checkEN[i], Qt::AlignCenter);
		widgetEN[i]->setLayout(hLayoutEN[i]);
		uitabW_PgmArg->setCellWidget(j, 0, widgetEN[i]);
		if (Qt::Checked != checkEN[i]->checkState()) {
			checkEN[0]->setCheckState((0) ? (Qt::Checked) : (Qt::Unchecked));
		}

		uitabW_PgmArg->setItem(j, 1, new QTableWidgetItem(QString("1")));
		uitabW_PgmArg->item(j, 1)->setTextAlignment(Qt::AlignCenter);

		uitabW_PgmArg->setItem(j, 2, new QTableWidgetItem(QString("cctv")));
		uitabW_PgmArg->item(j, 2)->setTextAlignment(Qt::AlignCenter);

		uitabW_PgmArg->setItem(j, 3, new QTableWidgetItem(QString("256,257,258,259")));
		uitabW_PgmArg->item(j, 3)->setTextAlignment(Qt::AlignCenter);
		j++;
	}
	connect(checkEN[0], SIGNAL(stateChanged(int)), this, SLOT(slot_check0_stateChanged(int)));
	uitabW_PgmArg->show();
	return 0;
}

int Hdmioptiongui::read_IP_Port(void)
{
	u8 tmp[6] = { 0 };
	tbs.target_ext_memory_rd_wt(READ, 0x08, tmp, 6);
	QString qstip = QString("%1:%2:%3:%4").arg(tmp[0])\
		.arg(tmp[1]).arg(tmp[2]).arg(tmp[3]);
	ui->lin_LIP->setText(qstip);
	Myswap(tmp[4], tmp[5]);
	ui->lin_LPort->setText(QString("%1").arg(*(u16*)&tmp[4]));
	return 0;
}

int Hdmioptiongui::recvNetTStoLocalFile(void)
{
	char recvtsbuf[MAXTSSIZE] = { '\0' };
	int n = recvfrom(udpfd, recvtsbuf, MAXTSSIZE, 0, (struct sockaddr*)&udpsockaddr, &len);
	if (n < 0) {
		qDebug() << "recv ts  time out";
		return -1;
	}
	FILE *pfTsFile = fopen(".tbsts", "wb+");
	if (NULL == pfTsFile){
		qDebug(".tbsts can not be open");
		return 0;
	}
	fwrite(recvtsbuf, MAXTSSIZE,1, pfTsFile);
	::fclose(pfTsFile);
	return 0;
}


bool Hdmioptiongui::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonDblClick) {
		if (watched == ui->widgetTitle) {
			on_btnMenu_Max_clicked();
			return true;
		}
	}

	return QWidget::eventFilter(watched, event);
}

void Hdmioptiongui::initForm()
{
	this->setProperty("form", true);
	this->setProperty("canMove", true);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);


	//QPixmap icon(":/image/logo.png");
	QPixmap icon(":/qss/psblack/uilogo/logo.png");
	ui->labIco->setPixmap(icon);
	ui->labIco->resize(icon.width(), icon.height());
	//IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf099), 30);
	IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xf068));
	IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xf067));
	IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d));

	//  IconHelper::Instance()->setIcon(ui->lab_enico, QChar(0xf069), 30);

	  //ui->widgetMenu->setVisible(false);
	ui->widgetTitle->installEventFilter(this);
	ui->widgetTitle->setProperty("form", "title");
	ui->widgetTop->setProperty("nav", "top");
	//ui->labTitle->setText("TBS CAPTURE");
	ui->labTitle->setFont(QFont("Microsoft Yahei", 20));
	this->setWindowTitle(ui->labTitle->text());

	//ui->stackedWidget->setStyleSheet("QLabel{font:10pt;}");

	//单独设置指示器大小
	int addWidth = 20;
	int addHeight = 10;
	int rbtnWidth = 15;
	int ckWidth = 13;
	int scrWidth = 12;
	int borderWidth = 3;

	QStringList qss;
	qss.append(QString("QComboBox::drop-down,QDateEdit::drop-down,QTimeEdit::drop-down,QDateTimeEdit::drop-down{width:%1px;}").arg(addWidth));
	qss.append(QString("QComboBox::down-arrow,QDateEdit[calendarPopup=\"true\"]::down-arrow,QTimeEdit[calendarPopup=\"true\"]::down-arrow,"
		"QDateTimeEdit[calendarPopup=\"true\"]::down-arrow{width:%1px;height:%1px;right:2px;}").arg(addHeight));
	qss.append(QString("QRadioButton::indicator{width:%1px;height:%1px;}").arg(rbtnWidth));
	qss.append(QString("QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:%1px;height:%1px;}").arg(ckWidth));
	qss.append(QString("QScrollBar:horizontal{min-height:%1px;border-radius:%2px;}QScrollBar::handle:horizontal{border-radius:%2px;}"
		"QScrollBar:vertical{min-width:%1px;border-radius:%2px;}QScrollBar::handle:vertical{border-radius:%2px;}").arg(scrWidth).arg(scrWidth / 2));
	qss.append(QString("QWidget#widget_top>QToolButton:pressed,QWidget#widget_top>QToolButton:hover,"
		"QWidget#widget_top>QToolButton:checked,QWidget#widget_top>QLabel:hover{"
		"border-width:0px 0px %1px 0px;}").arg(borderWidth));
	qss.append(QString("QWidget#widgetleft>QPushButton:checked,QWidget#widgetleft>QToolButton:checked,"
		"QWidget#widgetleft>QPushButton:pressed,QWidget#widgetleft>QToolButton:pressed{"
		"border-width:0px 0px 0px %1px;}").arg(borderWidth));


	this->setStyleSheet(qss.join(""));

	//QMessageBox{ color: red }
	//设置MODE按钮
	//QList<QRadioButton *> rads = ui->gro_Mode->findChildren<QRadioButton *>();
	//foreach (QRadioButton *rd, rads) {
   //     rd->setCheckable(true);
  //      connect(rd, SIGNAL(clicked()), this, SLOT(radioClick()));
   // }
	//设置Manual按钮

   // connect(ui->tree_Dev,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(showSelectedImage(QTreeWidgetItem*,int)));
	loadflg = 0;
	th = new threadTbs();
	connect(th, SIGNAL(sendMsg(Msg*)), this, SLOT(show_status(Msg*)));
	connect(ui->treeW_RFn, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showSelectedImage(QTreeWidgetItem*, int)));

	th->start();
	width_s = this->size().width();
	height_s = this->size().height();
	width = width_s;
	height = height_s;
	fontsize = 0;
	pgnum = 8;
	ui->tw_Setting->setCurrentIndex(0);
	for (int i = 0; i < CHECKNUM; i++) {
		checkEN[i] = NULL;
		hLayoutEN[i] = NULL;
		widgetEN[i] = NULL;
	}
	uitabW_PgmArg = ui->tabW_PgmArg;
	initStreamFrom();
}


void Hdmioptiongui::show_status(Msg *g)
{
	qDebug() << "show";
	int i = 0;
#if 1
	if (1 == g->type) {
		//test
		qbox = new  QMessageBox();
		qbox->setWindowTitle(tr("information"));
		qbox->setText(QString("Device is configuring,please wait.."));
		qbox->setStandardButtons(0);
		qbox->setFont(QFont("Microsoft Yahei", fontsize + 9));
		qbox->setProperty("Form", true);
		qbox->exec();
		return;
	}
	if (2 == g->type) {
		qbox->close();
		delete qbox;
		return;
	}

	if (4 == g->type) {
		if (QMessageBox::Ok == QMessageBox::information(this, tr("information"),
			tr("Success                                    "),
			QMessageBox::Ok,
			QMessageBox::Ok)) {
			readBuffer();
			tbs.HardRST(i);
			//	netf[col_n]->ipaddr = GetIp.section(':', 0, 0).trimmed();
			//	netf[col_n]->ipport = GetIp.section(':', 1, 1).trimmed().toInt();
				//qDebug() << "netfip:" << netf[col_n]->ipaddr << ":" << netf[col_n]->ipport;
				//udpsockaddr.sin_port = htons(netf[col_n]->ipport);
				//udpsockaddr.sin_addr.s_addr = inet_addr(netf[col_n]->ipaddr.toLatin1().data());

			loadflg = 0;
			return;
		}
		return;
	}
#endif
}


void Hdmioptiongui::on_btnMenu_Min_clicked()
{
	showMinimized();
}

void Hdmioptiongui::on_btnMenu_Max_clicked()
{
	static bool max = false;
	static QRect location = this->geometry();

	if (max) {
		this->setGeometry(location);
		width = width_s;
		height = height_s;
	}
	else {
		location = this->geometry();
		this->setGeometry(qApp->desktop()->availableGeometry());
		width = this->size().width();
		height = this->size().height();
	}

	this->setProperty("canMove", max);
	max = !max;
}

void Hdmioptiongui::on_btnMenu_Close_clicked()
{
	close();
}

void Hdmioptiongui::on_tw_Setting_currentChanged(int val)
{
	qDebug("val = %d ", val);
	twArg_flg = val;
}

void Hdmioptiongui::on_btn_Read_clicked()
{
	/**
	QString qstip = ui->lin_LIP->text();
	int intport =  ui->lin_LPort->text().toInt();
	udpfd = tbs.udpClose(udpfd);
	udpfd = tbs.udpOpen(qstip,intport);
	if (udpfd < 3) {
		if (QMessageBox::Yes == QMessageBox::warning(this, tr("warning"),
			tr("Network device connect fail             "),
			QMessageBox::Yes,
			QMessageBox::Yes)) {
			return;
		}
		return;
	}
	*/
	//readBuffer();
	//udpfd = tbs.udpClose(udpfd);
	/****test*************/
	QString qstip = ui->lin_LIP->text();
	int intport = ui->lin_LPort->text().toInt();
	udpfd = tbs.udpClose(udpfd);
	udpfd = tbs.udpOpen(qstip, intport);
	if (udpfd < 3) {
		if (QMessageBox::Yes == QMessageBox::warning(this, tr("warning"),
			tr("Network device connect fail             "),
			QMessageBox::Yes,
			QMessageBox::Yes)) {
			return;
		}
		return;
	}
	typeId = TYPEID;
	u8 tmp[8] = { 0 };
	//tbs.REG64_rd(0x8000,8,tmp);
	tbs.target_ext_memory_rd_wt(READ, 0x0000 + 0x08, tmp, 4);

	for (int i = 0; i < 4; i++) {
		qDebug("tmp[%d] = %#02x", i, tmp[i]);
	}
	udpfd = tbs.udpClose(udpfd);
	typeId = 0;
	return;
}

void Hdmioptiongui::on_btn_Write_clicked()
{

	QString qstip = ui->lin_LIP->text();
	int intport = ui->lin_LPort->text().toInt();
	udpfd = tbs.udpClose(udpfd);
	udpfd = tbs.udpOpen(qstip, intport);
	if (udpfd < 3) {
		if (QMessageBox::Yes == QMessageBox::warning(this, tr("warning"),
			tr("Network device connect fail             "),
			QMessageBox::Yes,
			QMessageBox::Yes)) {
			return;
		}
		return;
	}
	//ip port
	char *chip = qstip.toLatin1().data();
	sscanf(chip, "%d.%d.%d.%d", &ip_port_arry[0],
		&ip_port_arry[1], &ip_port_arry[2], &ip_port_arry[3]);
	ip_port_arry[4] = intport / 0x0ff;
	ip_port_arry[5] = intport % 0x0ff;

	//RF Port & TP Parameter Setting:
	//Uplink frequency"<<"Downlink frequency"
	//<<"Switch frequency"<<"Frequency" <<
	//"Symbolrate" << "Polarity" << "Diseqc"
	for (int i = 0; i < 8; i++) {
		streamarg[i].rtp_gate = 0;
		streamarg[i].all_ts_through = 0;
		streamarg[i].rtp_type = 0;
		streamarg[i].ip_addr = QString("192.168.8.188");
		streamarg[i].udp_port = 5442;
		streamarg[i].vpid = 256;
		streamarg[i].apid = 257;
		streamarg[i].pcr = 258;
		streamarg[i].pmt = 259;
		streamarg[i].ts_port = i;
		streamarg[i].pid_en = streamarg[i].all_ts_through ? 0xff : 0x00;
	}

	mode = 6;
	//udpfd = tbs.udpClose(udpfd);//??

	return;
}
void Hdmioptiongui::showSelectedImage(QTreeWidgetItem * item, int column)
{
	//遍历treeWidget
	QTreeWidgetItemIterator it(ui->treeW_RFn);
	while (*it) {
		if ((*it) == item) {
			(*it)->setCheckState(column, Qt::Checked);
		}
		else {
			(*it)->setCheckState(column, Qt::Unchecked);
		}
		++it;
	}
}
void Hdmioptiongui::slot_check0_stateChanged(int idx)
{
	for (int i = 1; i < (pgnum + 1); i++) {
		if (NULL != checkEN[i]) {
			checkEN[i]->setCheckState(idx ? Qt::Checked : Qt::Unchecked);
		}
	}
}
/**
void Hdmioptiongui::on_btn_Refresh_clicked()
{
	if (QMessageBox::No == QMessageBox::question(this, tr("question"),
		tr("Refresh Devices?                             "),
		QMessageBox::No | QMessageBox::Yes,
		QMessageBox::No)) {
		return;
	}
	typeId = 0;
	getNetTuners();
}
*/

int Hdmioptiongui::getNetTuners()
{
	return 0;
}




void Hdmioptiongui::on_sli_H_valueChanged(int value)
{
	qDebug() << value;
	fontsize = value;
	this->setStyleSheet(QString("font:%1pt;").arg(value));
	ui->labTitle->setFont(QFont("Microsoft Yahei", 9 + value));
	//qDebug()<<width<<"*"<<height;
	this->resize(width, height);
}
