#include "tbsfunc.h"
u8  tuner = 0;
u16 tbschipid = 0;
u8  rwbuf[RWNO] = {0};
int col = 0;
int col_n = 0;
u32 typeId = 0;
int twArg_flg = 0;
u8 loadflg = 0;
RF_TP_ARG rftp[8];
STREAM_ARG streamarg[8];
u8 ip_port_arry[8] = {0};

struct sockaddr_in udpsockaddr;
#ifdef Q_OS_WIN //windows
WSADATA wsaData;
SOCKET udpfd = 0;
int len = 0;
#else
int udpfd = 0;
socklen_t len = 0;
#endif
tbsfunc::tbsfunc()
{

}

tbsfunc::~tbsfunc()
{
}

void tbsfunc::REG64_rd(int subAddr, unsigned char num, unsigned char * rdbuffer)
{
	switch (typeId) {
	case TYPEID:
		udp_REG64_rd(subAddr, num, rdbuffer);
		break;
	default:
		break;
	}
	return;
}
void tbsfunc::REG64_wt(int subAddr, unsigned char num, unsigned char * wtbuffer)
{
	switch (typeId) {
	case TYPEID:
		udp_REG64_wt(subAddr, num, wtbuffer);
		break;
	default:
		break;
	}
	return;
}


unsigned char tbsfunc::check_host_free_REG64()
{
	unsigned char tmp_buff[8];
	int i = 0;
	
	do
	{
		REG64_rd(MAP_FPGA_BASEADDR + HOST_STATUS, 1, tmp_buff);
		//qDebug() <<"tmpbf = "<< tmp_buff[0];
		if ((tmp_buff[0] & 4) == 4)
		{
            QMSLEEP(2);
			i++;
		}
	} while (((tmp_buff[0] & 4) == 4) && (i<1500));  //2 second timeout;

	if (i == 1500)
		return 0;  //host busy and timeout;
	else
		return 1;
}
//--------------------------------------------------------------------------------------------

unsigned char tbsfunc::Check_host_status_REG64(unsigned char status)
{
	unsigned char tmp_buff[8] = {0};
	int i = 0;

	do
	{
		REG64_rd(MAP_FPGA_BASEADDR + HOST_STATUS, 8, tmp_buff);
		//if (1 == networkflg) {
		//	for(int i = 0;i < 8;i++)
			//	qDebug("tmp_buff[%d]:%#02x", i,tmp_buff[i]);
		//}
		if ((tmp_buff[0] & 1) != status)
		{
            QMSLEEP(3);
			i++;
		}

	} while (((tmp_buff[0] & 1) != status) && (i<1500));  //3 second timeout;

	if (i == 1500)
		return 0;  // timeout;
	else
		return 1;
}
//--------------------------------------------------------------------------------------------

unsigned char tbsfunc::read_from_external_memory(int addr, unsigned char num, unsigned char *buff)
{
	unsigned char i, tmp_buff[8];

	if (num == 0)
	{
		qDebug(" read_from_external_memory parameter error! [num == 0]");
		return 0;
	}
	else if (num > 8)
	{
		qDebug(" read_from_external_memory parameter error! [num >8]");
		return 0;
	}
	for (i = 0; i < num; i++)
		buff[i] = 0xff;

	//check_host_free;...............................
	if (check_host_free_REG64() == 0)
	{
		qDebug(" wait for host free timeout! ");
		//return 0;
	}

	//read tag;
	tmp_buff[0] = 0x31;
	tmp_buff[1] = num;
	tmp_buff[2] = (unsigned char)(addr >> 8);
	tmp_buff[3] = (unsigned char)(addr & 255);
	REG64_wt(MAP_FPGA_BASEADDR + WRITE_CMD, 4, tmp_buff);

	//check_host_active;...............................
	if (Check_host_status_REG64(0) == 0)
	{
		qDebug(" wait for host active timeout!");
		//return 0;
	}

	//release cs;
	REG64_wt(MAP_FPGA_BASEADDR + RELEASE_CS, 1, tmp_buff);
	//check host work done;...........................
	if (Check_host_status_REG64(1) == 0)
	{
		qDebug("wait for host work done timeout!");
		//return 0;
	}
	//read back;
	REG64_rd(MAP_FPGA_BASEADDR + READ_DATA, num, buff);

	return 0;
}
//--------------------------------------------------------------------------------------------

unsigned char tbsfunc::write_to_external_memory(int addr, unsigned char num, unsigned char *buff)
{
	unsigned char i, tmp_buff[8];

	if (num > 8)
	{
		qDebug(" write_to_external_memory parameter error! [num >8]");
		return 0;
	}

	//check_host_free;...............................
	if (check_host_free_REG64() == 0)
	{
		qDebug(" wait for host free timeout!");
		return 0;
	}

	if (num != 0)
	{
		for (i = 0; i < num; i++)
			tmp_buff[i] = buff[i];
		REG64_wt(MAP_FPGA_BASEADDR + WRITE_DATA, num, tmp_buff);
	}

	//write tag;
	tmp_buff[0] = 0x30;
	tmp_buff[1] = num;
	tmp_buff[2] = (unsigned char)(addr >> 8);
	tmp_buff[3] = (unsigned char)(addr & 255);
	REG64_wt(MAP_FPGA_BASEADDR + WRITE_CMD, 4, tmp_buff);

	//check_host_active;...............................
	if (Check_host_status_REG64(0) == 0)
	{
		qDebug(" wait for host active timeout!");
		return 1;
	}

	//release cs;
	REG64_wt(MAP_FPGA_BASEADDR + RELEASE_CS, 1, tmp_buff);
	if (Check_host_status_REG64(1) == 0)
	{
		qDebug("wait for host work done timeout!");
        return 1;
	}
    return 0;
}


void tbsfunc::HardRST(int i )
{
	u8 tmpbuf[8] = { 0xff,0xff,0xff,0xff};
	switch (typeId) {
	case TYPEID:
		REG64_wt(0xff00+0x08*i, 1, tmpbuf);
        QMSLEEP(100);
		tmpbuf[0] = 0;
		REG64_wt(0xff00 + 0x08 * i, 1, tmpbuf);
		break;
	default:
		break;
	}
	return;
}

bool tbsfunc::monopolizeCpu_w_REG64(int m_addr, unsigned char * wtbuff, unsigned char num)
{
	unsigned char i, tmp_buff[8];

	if (num >8)
	{
		qDebug(" write_to_external_memory parameter error! [num >8]");
		return 0;
	}

	//check_host_free;...............................
	if (check_host_free_REG64() == 0)
	{
		qDebug(" wait for host free timeout!");
		return 0;
	}

	if (num != 0)
	{
		for (i = 0; i<num; i++)
			tmp_buff[i] = wtbuff[i];
		REG64_wt(MAP_FPGA_BASEADDR + WRITE_DATA, num, tmp_buff);
	}

	//write tag;
	tmp_buff[0] = 0x32;
	tmp_buff[1] = num;
	tmp_buff[2] = (unsigned char)(m_addr >> 8);
	tmp_buff[3] = (unsigned char)(m_addr & 255);
	REG64_wt(MAP_FPGA_BASEADDR + WRITE_CMD, 4, tmp_buff);

	//check_host_active;...............................
	if (Check_host_status_REG64(0) == 0)
	{
		qDebug(" wait for host active timeout!");
		//return 0;
	}

	//release cs;
	REG64_wt(MAP_FPGA_BASEADDR + RELEASE_CS, 1, tmp_buff);
	//check host work done;...........................
	if (Check_host_status_REG64(1) == 0)
	{
		qDebug("wait for host work done timeout!");
		return 0;
	}

	return 1;
}

bool tbsfunc::monopolizeCpu_r_REG64(int m_addr, unsigned char * rdbuff, unsigned char num)
{
	unsigned char i, tmp_buff[8];

	if (num == 0)
	{
		qDebug(" read_from_external_memory parameter error! [num == 0]");
		return 0;
	}
	else if (num >8)
	{
		qDebug(" read_from_external_memory parameter error! [num >8]");
		return 0;
	}
	for (i = 0; i<num; i++)
		rdbuff[i] = 0xff;

	//check_host_free;...............................
	if (check_host_free_REG64() == 0)
	{
		qDebug(" wait for host free timeout! ");
		return 0;
	}

	//read tag;
	tmp_buff[0] = 0x33;
	tmp_buff[1] = num;
	tmp_buff[2] = (unsigned char)(m_addr >> 8);
	tmp_buff[3] = (unsigned char)(m_addr & 255);
	REG64_wt(MAP_FPGA_BASEADDR + WRITE_CMD, 4, tmp_buff);

	//check_host_active;...............................
	if (Check_host_status_REG64(0) == 0)
	{
		qDebug(" wait for host active timeout!");
		//return 0;
	}

	//release cs;
	REG64_wt(MAP_FPGA_BASEADDR + RELEASE_CS, 1, tmp_buff);

	//check host work done;...........................
	if (Check_host_status_REG64(1) == 0)
	{
		qDebug("wait for host work done timeout!");
		return 0;
	}

	//read back;
	REG64_rd(MAP_FPGA_BASEADDR + READ_DATA, num, rdbuff);

	return false;
}

int tbsfunc::reHdmi()
{
    u8 tmpbuf[4] = { 0 };
    target_ext_memory_rd_wt(WRITE, ADDRESS_HDMI(tuner) + 0x08, tmpbuf, 1);
	return 0;
}


void tbsfunc::write_M23Register(int num, int* m23_addr, int* m23_data)
{
	unsigned char tmpM23addr[4];
	unsigned char tmpM23data[4];
	int i = 0;

	for (i = 0; i < num; i++)
	{
		tmpM23addr[0] = (*(m23_addr + i) & 0xff00) >> 8;
		tmpM23addr[1] = *(m23_addr + i) & 0xff;
		tmpM23addr[2] = 0;
		tmpM23addr[3] = 0;


		tmpM23data[0] = (*(m23_data + i) & 0xff00) >> 8;
		tmpM23data[1] = *(m23_data + i) & 0xff;
		tmpM23data[2] = 0;
		tmpM23data[3] = 0;
		target_ext_memory_rd_wt(WRITE, 0x80 * tuner + 0x20 + i * 4, tmpM23addr, 2);
		target_ext_memory_rd_wt(WRITE, 0x80 * tuner + 0x22 + i * 4, tmpM23data, 2);
		qDebug("addr:%04x  data:%04x", *(u16*)tmpM23addr, *(u16*)tmpM23data);

	}

	return;
}

bool tbsfunc::monopolizeCpu_w(int m_addr, unsigned char * wtbuff, unsigned char num)
{
	switch (typeId) {
	case TYPEID:
		return monopolizeCpu_w_REG64(m_addr, wtbuff, num);
		break;
	default:
		break;
	}
    return 1;
}

bool tbsfunc::monopolizeCpu_r(int m_addr, unsigned char * rdbuff, unsigned char num)
{
	switch (typeId) {
	case TYPEID:
		return monopolizeCpu_r_REG64(m_addr, rdbuff, num);
		break;
	default:
		break;
	}
    return 1;
}

void tbsfunc::read_M23Register(int num, int* m23_data)
{
	unsigned char tmpM23data[4];
	int i = 0;

	for (i = 0; i < num; i++)
	{
		//read the data from register
		//read_24cxx( 0x22+i*4,2, tmpM23data);
		target_ext_memory_rd_wt(READ, 0x80 * tuner + 0x22 + i * 4, tmpM23data, 2);
		*(m23_data + i) = tmpM23data[1] & 0xff;
		*(m23_data + i) += ((tmpM23data[0] << 8) & 0xff00);
	}

	return;
}

//===............................................................................

unsigned char tbsfunc::ext_ram_write(int ram_addr, unsigned char *wtbff, int wt_size)
{
	int i, k, x8byte;
	unsigned char finishbyte;
	x8byte = wt_size / 8;
	finishbyte = (unsigned char)(wt_size & 7);

	//first send & remainder bytes.
	if (finishbyte == 0)
	{
		finishbyte = 8;
		x8byte -= 1;
	}
	ext_one_time_wt(ram_addr, wtbff, finishbyte);
	
	i = finishbyte;
	if (x8byte != 0)
	{
		for (k = 0; k < x8byte; k++)
			ext_one_time_wt((ram_addr + i + k * 8), &wtbff[i + k * 8], 8);
	}

    return 0;
}

//===............................................................................

unsigned char tbsfunc::ext_one_time_wt(int m_addr, unsigned char *wtbuff, unsigned char num) {
	switch (typeId) {
	case TYPEID:
		return write_to_external_memory(m_addr, num,wtbuff);
		break;
	default:
		break;
	}
	return 0;
}

unsigned char tbsfunc::ext_one_time_rd(int m_addr, unsigned char *rdbuff, unsigned char num) {
	switch (typeId) {
	case TYPEID:
		return read_from_external_memory(m_addr, num, rdbuff);
		break;
	default:
		break;
	}
	return 0;
}

//===............................................................................

unsigned char tbsfunc::ext_ram_read(int ram_addr, unsigned char *rdbff, int rd_size)
{
	int i, k, x8byte;
	unsigned char finishbyte, buff[8];
	// qDebug("ram_addr:%#x\n",ram_addr);
	x8byte = rd_size / 8;
	finishbyte = (unsigned char)(rd_size & 7);
	//first send & remainder bytes.
	if (finishbyte == 0)
	{
		finishbyte = 8;
		x8byte -= 1;
	}
	ext_one_time_rd(ram_addr, rdbff, finishbyte);

	i = finishbyte;
	if (x8byte != 0)
	{
		for (k = 0; k < x8byte; k++)
			ext_one_time_rd((ram_addr + i + k * 8), &rdbff[i + k * 8], 8);
	}

    return 0;
}

unsigned char tbsfunc::target_ext_memory_rd_wt(unsigned char tg_mode,
	int tg_addr,
	unsigned char *buff,
	int tg_size)
{
	if (tg_mode == READ)
		return ext_ram_read(tg_addr, buff, tg_size);
	else if (tg_mode == WRITE)
		return ext_ram_write(tg_addr, buff, tg_size);
	else
		qDebug("[target_external_memory_rd_wt] read write mode error!");
    return 0;
}



int tbsfunc::reRunNetWork()
{
	//fe00  ==> rerunnetwork
	u8 tmpbuf[4] = { 0 };
	target_ext_memory_rd_wt(WRITE, 0xff10, tmpbuf, 1);
	return 0;
}


int tbsfunc::reRtpUdpStreaming()
{
	u8 tmpbuf[4] = { 0 };
	target_ext_memory_rd_wt(WRITE, 0xff18, tmpbuf, 1);

	return 0;
}



void tbsfunc::udp_REG64_rd(int subAddr, unsigned char num, unsigned char * rdbuffer)
{
	char sendbuff[64] = { 0 };
	char recvbuff[64] = { 0 };
    len = sizeof(udpsockaddr);
	int n = 0;
	int j = 0;
	// 27bit address
	sendbuff[0] = 0xc0+ (u8)((subAddr & 0x07000000) >> 24);
	sendbuff[1] = (u8)((subAddr & 0x00ff0000) >> 16);
	sendbuff[2] = (u8)((subAddr & 0x0000ff00) >> 8);
	sendbuff[3] = (u8)((subAddr & 0x000000ff));

	//sendbuff[2] = 0x80;
	//sendbuff[3] = 0x20;
	n = sendto(udpfd, sendbuff, 64, 0, (struct sockaddr*)&udpsockaddr, len);
	if (n < 0) {
		qDebug() << "udp_REG64_rd:sendto time out";
		return ;
	}
	n = recvfrom(udpfd, recvbuff, 64, 0, (struct sockaddr*)&udpsockaddr, &len);
	if (n < 0) {
		qDebug() << "udp_REG64_rd:recvfrom3 time out";
		return ;
	}
	for (int i = 0; i < 12; i++) {
		//qDebug("udp_REG64_rd recv:%d=%02x", i, (u8)recvbuff[i]);
		if ((i > 3) &&(j < num)) {
			rdbuffer[j] = (u8)recvbuff[i];
			j++;
		}
	}
	return;
}

void tbsfunc::udp_REG64_wt(int subAddr, unsigned char num, unsigned char * wtbuffer)
{
	char sendbuff[64] = { 0 };
	char recvbuff[64] = { 0 };
    len = sizeof(udpsockaddr);
	int n = 0;
	int i = 0;
	int j = 0;

	// 27bit address
	sendbuff[0] = 0x00 + (u8)((subAddr & 0x07000000) >> 24);
	sendbuff[1] = (u8)((subAddr & 0x00ff0000) >> 16);
	sendbuff[2] = (u8)((subAddr & 0x0000ff00) >> 8);
	sendbuff[3] = (u8)((subAddr & 0x000000ff));

	for (i = 0; i < num;i++) {
		sendbuff[i+4] = wtbuffer[i];
	}
	n = sendto(udpfd, sendbuff, 64, 0, (struct sockaddr*)&udpsockaddr, len);
	if (n < 0) {
		qDebug() << "udp_REG64_wt:sendto time out";
		return ;
	}

	memset(recvbuff, 0, 64);
	n = recvfrom(udpfd, recvbuff, 64, 0, (struct sockaddr*)&udpsockaddr, &len);
	if (n < 0) {
		qDebug() << "udp_REG64_wt:recvfrom time out";
		return ;
	}
	for (int i = 0; i < 12; i++) {
		//qDebug("udp_REG64_wt:recv:%d=%02x", i, (u8)recvbuff[i]);
	}
	return ;
}

#ifdef Q_OS_WIN //windows
SOCKET tbsfunc::udpOpen(QString ipaddr, int ipport)
{
	SOCKET ufd;
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	int timeout = 5000;
#else
int tbsfunc::udpOpen(QString ipaddr, int ipport)
{
	int ufd = 0;
	struct timeval timeout = { 5,0 };
#endif
	char sendbuff[64] = { 0 };
	char recvbuff[64] = { 0 };
	/*create Socket*/
	if ((ufd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		qDebug() << "fail to socket";
		return 0;
	}
	/*set sockaddr_in*/
	memset(&udpsockaddr, 0, sizeof(udpsockaddr));
	udpsockaddr.sin_family = AF_INET;
	udpsockaddr.sin_port = htons(ipport);
	udpsockaddr.sin_addr.s_addr = inet_addr(ipaddr.toStdString().data());
	len = sizeof(udpsockaddr);
	setsockopt(ufd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	setsockopt(ufd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
	int n = 0;
	/*
	//1.begin w 0x0fff5
	sendbuff[0] = 0x00;
	sendbuff[1] = 0x00;
	sendbuff[2] = 0xff;
	sendbuff[3] = 0xf5;

	n = sendto(ufd, sendbuff, 64, 0, (struct sockaddr*)&udpsockaddr, len);
	if (n < 0) {
		qDebug() << "sendto1 time out";
		return 0;
	}
	memset(recvbuff, 0, 64);
	n = recvfrom(ufd, recvbuff, 64, 0, (struct sockaddr*)&udpsockaddr, &len);
	if (n < 0) {
		qDebug() << "recvfrom1 time out";
		return 0;
	}
	
	//2.r 0x0fff5
	sendbuff[0] = 0xc0;
	sendbuff[1] = 0x00;
	sendbuff[2] = 0xff;
	sendbuff[3] = 0xf5;

	n = sendto(ufd, sendbuff, 64, 0, (struct sockaddr*)&udpsockaddr, len);
	if (n < 0) {
	qDebug() << "sendto2 time out";
	return 0;
	}

	//3.wait 3s=> recv 64'h8080_8080_8080_8080 is ok,64'h0505_0505_0505_0505 is busy
	memset(recvbuff, 0, 64);
	n = recvfrom(ufd, recvbuff, 64, 0, (struct sockaddr*)&udpsockaddr, &len);
	if (n < 0) {
	qDebug() << "recvfrom2 time out";
	return 0;
	}
	u8 tmpbuf[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	int j = 0;
	for (int i = 0; i < 12; i++) {
	if (tmpbuf[j] == (u8)recvbuff[i]) {
	if (8 == ++j) {
	qDebug() << "Getting right to use is successful";
	break;
	}
	}
	}
	if (j < 8) {
	qDebug() << "Getting right to use is failed ";
	return 0;
	}
	*/
	return ufd;
}

#ifdef Q_OS_WIN //windows
SOCKET tbsfunc::udpClose(SOCKET ufd)
{
#else
int tbsfunc::udpClose(int ufd)
{
#endif
	int n = 0;
	char sendbuff[64] = { 0 };
	char recvbuff[64] = { 0 };
	len = sizeof(udpsockaddr);
	if (ufd > 2) {
		
		/*
		//5.end=> w 0xfffa free dev
		sendbuff[0] = 0x00;
		sendbuff[1] = 0x00;
		sendbuff[2] = 0xff;
		sendbuff[3] = 0xfa;
		n = sendto(ufd, sendbuff, 64, 0, (struct sockaddr*)&udpsockaddr, len);
		if (n < 0) {
			qDebug() << "sendto4 time out";
			return 0;
		}

		memset(recvbuff, 0, 64);
		n = recvfrom(ufd, recvbuff, 64, 0, (struct sockaddr*)&udpsockaddr, &len);
		if (n < 0) {
			qDebug() << "recvfrom4 time out";
			return 0;
		}
		*/
#ifdef Q_OS_WIN //windows
		closesocket(ufd);
		WSACleanup();
#else
		::close(ufd);
#endif
		return 0;
	}
	return ufd;
}
