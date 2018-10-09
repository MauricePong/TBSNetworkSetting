#include "threadtbs.h"
volatile bool swflg = 0;
volatile u8 mode = 0;

threadTbs::threadTbs() :QThread()
{
	swflg = 0;
	m = new Msg();
	m->ok = 0;
	m->type = 0;
	m->progress = 0;
}

threadTbs::~threadTbs()
{

}

void threadTbs::writeBuffer()
{

	m->ok = 1;
	m->type = 1;
	m->progress = 0;
	emit sendMsg(m);



	m->ok = 1;
	m->type = 2;
	m->progress = 0;
	emit sendMsg(m);
	QMSLEEP(1);
	m->ok = 1;
	m->type = 4;
	m->progress = 0;
	emit sendMsg(m);
	return;
}

void threadTbs::writeIPandPort()
{
	//Myswap(ip_port_arry[4], ip_port_arry[5]);
	qDebug("local ip : port =>%d.%d.%d.%d:(H=%d;L = %d)", ip_port_arry[0],
		ip_port_arry[1], ip_port_arry[2],
		ip_port_arry[3], ip_port_arry[4], ip_port_arry[5]);
	et.target_ext_memory_rd_wt(WRITE, 0x0000 + 0x08, ip_port_arry, 6);
}

void threadTbs::lockTable()
{
	int i = 0;
	for (i = 0; i < 8; i++) {
		u8 status = 0;
		u32	Freq_KHz = 0;
		u32	SymRate_KSps = 0;
		bool V_polar = 0;
		bool _22K_on = 0;
		u8	tmp[33];
		u8 u_Pol = rftp[i].tp_hv;
		u8 u_diseq = rftp[i].tp_hv;
		int ll_lnbhighfreq = rftp[i].upfreq;
		int ll_lnblowfreq = rftp[i].downfreq;
		int ll_lnbswitchfreq = rftp[i].switchfreq;
		int ll_lnbfreqkHz = rftp[i].tp_if;
		int ll_symbolrate = rftp[i].tp_symbolrate;
		bool hiband = (ll_lnbfreqkHz > ll_lnbswitchfreq);
		ll_lnbfreqkHz -= (hiband ? ll_lnbhighfreq : ll_lnblowfreq);
		if (ll_lnbfreqkHz < 0) {
			ll_lnbfreqkHz = -ll_lnbfreqkHz;
		}
		qDebug("tp freq = %d, symb = %d, pol= %d, 22k = %d",
			ll_lnbfreqkHz * 1000, ll_symbolrate, u_Pol, hiband);
		//TP_obj.Freq_KHz = 1238000;
		//TP_obj.SymRate_KSps = 41250;
		Freq_KHz = ll_lnbfreqkHz * 1000;
		SymRate_KSps = ll_symbolrate;
		V_polar = u_Pol;
		if (hiband)
			_22K_on = 1;
		else
			_22K_on = 0;

		tmp[0x0e] = 0xe0;
		tmp[0x0f] = 0x10;
		tmp[0x10] = 0x39;
		tmp[0x11] = 0xf0;
		if (u_diseq > 0)
		{

			if (hiband)//modified 2011 08 08 liuy
			{
				tmp[0x11] += 0x01;
			}
			// polarzation; if (V or L) bit1 = 0;//当H=0 第二位变一
			if (u_Pol == 0)
			{
				tmp[0x11] += 0x02;
			}
			//satellite position and option; bit2 and bit3;	
			switch (u_diseq)
			{
			case 1:
				tmp[0x11] &= 0xf3;	//0xf0,0xf1,0xf2,0xf3;
				break;
			case 2:
				tmp[0x11] |= 0x04; // set position to 1			//0xf4,0xf5,0xf6,0xf7;
				break;
			case 3:
				tmp[0x11] |= 0x08; // set option to 1			//0xf8,0xf9,0xfa,0xfb;
				break;
			case 4:
				tmp[0x11] |= 0x04; // set position to 1			//0xfc,0xfd,0xfe,0xff;
				tmp[0x11] |= 0x08; // set option to 1
				break;
			default:
				tmp[0x11] = 0xf0;
			}

			tmp[0x0b] = 1; //lnb power on;
			tmp[0x0c] = _22K_on; // PORT_22KHz, 22lhz on;
			tmp[0x0d] = 4;

			tmp[0] = 1;	//open 
			tmp[1] = (u8)(Freq_KHz >> 24 & 0xFF); //IF = 1238000L
			tmp[2] = (u8)(Freq_KHz >> 16 & 0xFF);
			tmp[3] = (u8)(Freq_KHz >> 8 & 0xFF);
			tmp[4] = (u8)(Freq_KHz & 0xFF); // low

			tmp[5] = (u8)(SymRate_KSps >> 24 & 0xFF); //symbolrate = 41250L
			tmp[6] = (u8)(SymRate_KSps >> 16 & 0xFF);
			tmp[7] = (u8)(SymRate_KSps >> 8 & 0xFF);
			tmp[8] = (u8)(SymRate_KSps & 0xFF); // low

			tmp[9] = V_polar;	//v              // TP_obj.V_polar
			tmp[0x0a] = _22K_on;	 //22khz on   // TP_obj._22K_on
			et.target_ext_memory_rd_wt(WRITE, 0x0100 + 0x10 * i, tmp, 32);

			for (int i = 0; i < 10; i++)
			{
				QMSLEEP(100);
				et.REG64_rd( 0xff40 + 0x08 * i, 1, &status);
				if (status == 6)
				{
					qDebug("Signal Locked is OK \n");
					break;
				}
			}
			if (status != 6) {
				qDebug("Signal Locked is fail \n");
			}
			qDebug("Tuner %d : lock status = %x",i, status);
		}
	}
}

void threadTbs::writeStreamArg()
{
	int i = 0;
	int n = 8;
	for (i = 0; i < 8; i++) {
		u8 tmp[24] = { 0 };
		tmp[0] = (((streamarg[i].rtp_gate & 0x01) << 7) & 0x80);
		tmp[0] |= (((streamarg[i].all_ts_through & 0x01) << 6) & 0x40);
		tmp[0] |= (((streamarg[i].rtp_type & 0x01) << 5) & 0x20);
		tmp[0] |= (streamarg[i].ts_port & 0x03);

		tmp[1] = streamarg[i].pid_en;
		tmp[2] = streamarg[i].udp_port / 0x0ff;
		tmp[3] = streamarg[i].udp_port % 0x0ff;
		char *chip = streamarg[i].ip_addr.toLatin1().data();
		sscanf(chip,"%d.%d.%d.%d",&tmp[4], &tmp[5], &tmp[6], &tmp[7]);
		tmp[8] = streamarg[i].vpid / 0x0ff;
		tmp[9] = streamarg[i].vpid % 0x0ff;
		tmp[10] = streamarg[i].apid / 0x0ff;
		tmp[11] = streamarg[i].apid % 0x0ff;
		tmp[12] = streamarg[i].pcr / 0x0ff;
		tmp[13] = streamarg[i].pcr % 0x0ff;
		tmp[14] = streamarg[i].pmt / 0x0ff;
		tmp[15] = streamarg[i].pmt % 0x0ff;
		et.target_ext_memory_rd_wt(WRITE, 0x0300 + 24*n* i, tmp, 32);
	}
}


void threadTbs::udpMulticastClinet()
{
#ifdef Q_OS_WIN //windows
	SOCKET  Mfd;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	int timeout = 2000;

#else           //linux
	int Mfd;
	struct timeval timeout = { 2,0 };
#endif
	struct sockaddr_in addr;
	char recvtmp[64] = { '\0' };
	char sendbuf[64] = { '\0' };
	char recvbuf[64] = { '\0' };
	u8 tmp[2] = { 0 };
	if ((Mfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		qDebug() << "fail to socket";
		return;
}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("239.255.0.9");
	addr.sin_port = htons(21571);//0x5443
	len = sizeof(addr);
	setsockopt(Mfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	setsockopt(Mfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
	sendbuf[0] = 0xc0;//read
	sendbuf[1] = 0xff;
	sendbuf[2] = 0xff;
	sendbuf[3] = 0xff;
	sendbuf[4] = 0x54;
	sendbuf[5] = 0x42;
	sendbuf[6] = 0x53;
	sendbuf[7] = 0x50;
	sendbuf[8] = 0x52;
	sendbuf[9] = 0x4f;
	sendbuf[10] = 0x44;
	sendbuf[11] = 0x54;
	if (sendto(Mfd, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &addr, len) < 0)
	{
		qDebug() << " fail to sendto";
		return;
	}
	int i = 0;
	while (1) {
		int n = recvfrom(Mfd, recvbuf, 64, 0, (struct sockaddr *)&addr, &len);
		if (n < 0) {
			qDebug() << "scan ip dev over";
			break;
		}
		else {
			if (((u8)(0x53) != (u8)recvbuf[4]) || ((u8)(0x01) != (u8)recvbuf[5])) {
				continue;
			}
			tmp[0] = (u8)recvbuf[11];
			tmp[1] = (u8)recvbuf[10];
			sprintf(recvtmp, "TBS%02x%02x:%d.%d.%d.%d:%d", (u8)recvbuf[4], (u8)recvbuf[5], (u8)recvbuf[6], \
				(u8)recvbuf[7], (u8)recvbuf[8], (u8)recvbuf[9], *(u16*)&tmp[0]);
			emit sendIp(QString(recvtmp), i);
			i++;
		}
		memset(recvbuf, 0, sizeof(recvbuf));
		QMSLEEP(1);
	}
#ifdef Q_OS_WIN //windows
	closesocket(Mfd);
	WSACleanup();
#else           //linux
	::close(Mfd);
#endif
	return;
}


void threadTbs::run()
{
	while (1 != swflg) {
		if (mode == 0) {
			QMSLEEP(1);
			continue;
		}
		switch (mode) {
		case 6:
			writeBuffer();
			break;
		case 7:
			udpMulticastClinet();
			break;
		}
		mode = 0;
	}
	delete m;
	this->quit();
	this->wait();
}
