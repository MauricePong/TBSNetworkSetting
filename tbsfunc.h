#ifndef TBSFUNC_H
#define TBSFUNC_H
#define NOMINMAX
#define RWNO  128
#define CHECKNUM 9
#include <QDebug>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QtNetwork>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QProcess>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef Q_OS_WIN //windows
#include <windows.h>
#include <conio.h>
#include <initguid.h>
#include <ks.h>
#include <ksmedia.h>
#include <bdatypes.h>
#include <bdamedia.h>
#include <bdaiface.h>
#include <tuner.h>
//#include <atlbase.h>
#include <comdef.h>
#include <winsock.h>
#define  QMSLEEP(n)  Sleep(n)
//net
extern SOCKET udpfd;
extern WSADATA wsaData;
extern int len;
#else           //linux
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fs.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <linux/dvb/frontend.h>
#define  QMSLEEP(n)  usleep(1000*n)
extern int udpfd;
extern socklen_t len;
#endif

extern "C" {
//#include "ParseTS_Length.h"
//#include "ParsePAT.h"
//#include "ParsePMT.h"
//#include "ParseSDT.h"
//#include "ParseEIT.h"
//#include "ParseNIT.h"
//#include "Program.h"
#include "MainTest.h"

}

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern u8  tuner;
extern u16 tbschipid;
extern u8 rwbuf[RWNO];
extern int twArg_flg;
extern struct sockaddr_in udpsockaddr;

#ifndef WRITE
#define WRITE            0
#endif
#ifndef READ
#define READ             1
#endif
//spi flash
#define SPI_CONFIG      	 0x10
#define SPI_STATUS      	 0x10
#define SPI_COMMAND     	 0x14
#define SPI_WT_DATA     	 0x18
#define SPI_RD_DATA     	 0x1c
#define SPI_ENABLE       	 0x1c
#define GPIO_VERSION         0x20
//HDMI the folloing address( Write:User input video format, Read: true format from 6604)   very import: High and low is reversed
#define HDMI_RST  		0x44      //system reset
#define HDMI_AUTO_MODE  0x48  	  //Auto_mode (1) meaningful. System to identify video format parameters or user input video format parameters
#define HDMI_HYNC_VYNC  0x48      //bit[31:16]Hync, bit[15:0]Vync
#define HDMI_SCAN_FREQ	0x4c      //bit[31:24] Video format frequency parameters, Bit[16]: user input video format is  (0:interleave): 1:progressive) scanning
//===============================================
//-------------24cXX --------------------------------------------------------
#define BASE_ADDRESS_24CXX  	0x4000
#define CMD_24CXX	      0*4   //write only
#define DATA0_24CXX       1*4   //W/R
#define DATA1_24CXX       2*4   //W/R
#define CS_RELEASE        3*4   //write only

#define STATUS_MAC16_24CXX	 0*4   //read only
#define MAC32_24CXX	 		 3*4   //read only


#define  ADDRESS_HDMI(n)  (0xff00+0x40 *(n))
#define  ADDRESS_HDMI0    0xff00
#define  ADDRESS_HDMI1    0xff40
#define  ADDRESS_HDMI2    0xff80
#define  ADDRESS_HDMI3    0xffc0
#define  OFFSET_START     0*0


#define GPIOBASE_ADDR     0x0000
#define CHIP_OFFSET_ADDR  4*8
//pid and program_number
#define  PID(n)           (0x70+(n)*0x80)
#define  PROGRAM_START_NO(n) (0x0f+0x80*(n))

// HardRST
#define  BASE_ADDRESS_HARDRST          0x0000
#define  OFFSET_ADDRESS_HARDRST        1*4
//#define
//mac:
#define MAC_ADDRESS_2     0*4
#define MAC_ADDRESS_4     3*4

#define TBS5301ID            (0x0153)
#define TBS6304ID            (0x0463)
#define TBS6301ID            (0x0163)
#define DEVAPI_MAX_DEVICES   32


#define MAP_FPGA_BASEADDR  	0x4000//write 
#define WRITE_CMD 			0*8	
#define WRITE_DATA 			1*8
#define RELEASE_CS 			2*8
//read
#define HOST_STATUS 		0*8
// {5'd0,mcu_busy,write_error,mcu_done,56'd0};
#define READ_DATA 			1*8
#define READ_MAC 			2*8

#define AUDIIO_SELECT_OFFSETADDR        0x17     //bit[56]
#define AUDIIO_FREQ_SELECT_OFFSETADDR   0x18     //bit[48:49]

#define  BASE_SETTING_NET_ADDR			0x0100
#define  NET_LOCAL_MAC_OFFSETADDR		0x00   //0x00 ~ 0x05
#define  NET_LOCAL_IP_OFFSETADDR        0x08   //0x08~0x0b
#define  NET_SERVER_PORT_OFFSETADDR     0x0c   //0x0c~0x0d
#define  SELECT_UDP_OR_RTP_OFFSETADDR   0x0e
//0x10~0x27:	one RTP parameter(Total 24 bytes)
#define  RTP_PARAMETER_OFFSETADDR       0x10  
/*
	rtp_gate;  		//bit 7;
	all_ts_through;  	//bit6
	position;			//bit5~3
	ts_port;			//bit2~0
*/
#define  RTP_PID_EN_OFFSETADDR	        0x11
#define  UDP_PORT_OFFSETADDR	        0x12   //2 bytes
#define  RTP_IP_ADDR_OFFSETADDR	        0x14   //4 bytes
#define  RTP_TV_PID_OFFSETADDR	        0x18   //16 bytes

#define  I2C_CHIP_ADDRESS               0x50
#define  BUS_IS_FREE_OFFSETADDR         0x02
#define  CHECK_I2C_BUS_ADDR             0x11
#define  RE_RUN_NETWORK_ADDR            0xfe00

#define Myswap(a,b)  do{ a = a^b; b = a^b; a = a^b;}while(0)
#define TYPEID     5301
#define MAXTSSIZE  (188*7)
//NetWorkaddr

class Msg {
public:
	bool ok;
	u8 type;
	u32 progress;
};

/*
Base Network Setting：
Offset Address:
#define LOCAL_MAC_ADDR		0x00
#define LOCAL_IP_ADDR		0x08
#define UDP_PORT_ADDR		0x0c

RF Port & TP Parameter Setting:
TP Offset Address:
#define TP_GATE						0x00
#define TP_IF							0x01   //0x01~0x04
#define TP_SYMBOLRATE				0x05   //0x05~0x08
#define TP_H_V						0x09
#define TP_22kHz						0x0a
RF Port Offset Address:
#define LNB_Power						0x0b  //bit7
#define RF_22kHZ						0x0c  //bit7
#define DiSEqC_SIZE					0x0d  //8 bit
#define DiSEqC_DATA					0x0e~0x1f  //18 bytes


Total 24 bytes in 24c32;
*Note:
u8 	rtp_gate;  		//bit 7;
	all_ts_through;  	//bit6
	rtp_type;  	    //bit5  "1": rtp; 0: udp;
	reserved;			//bit4~3
	ts_port;			//bit2~0
u8  pid_en;   		//bit7~0
u8  udp_port[2];      //2 bytes
u8  ip_addr[4];		//4 bytes
u16  tv_pid[8];		//16 bytes

* */

class RF_TP_ARG {
public:
	//TP arg
	int upfreq;
	int downfreq;
	int switchfreq;
	int tp_if;
	int tp_symbolrate;
	u8 tp_hv;
	u8 diseqc_data;
};

class STREAM_ARG {
public:
	u8 	rtp_gate;
	u8  all_ts_through;
	u8  rtp_type;
	u8  ts_port;
	u8  pid_en;
	u16 udp_port;
	QString  ip_addr;
	u16 vpid;
	u16 apid;
	u16 pcr;
	u16 pmt;
};
typedef enum _GX1132_LOCK_STATUS
{
	UNLOCKED,
	AGC_LOCKED,
	BCS_LOCKED,
	TIM_LOCKED,
	CRL_LOCKED,
	EQU_LOCKED,
	FEC_LOCKED
}GX1132_LOCK_STATUS;


extern int col;
extern int col_n;
extern u32 typeId;
extern u8 loadflg;
extern RF_TP_ARG rftp[8];
extern STREAM_ARG streamarg[8];
extern u8 ip_port_arry[8];

class tbsfunc
{
public:
	explicit tbsfunc();
	~tbsfunc();
	void read_M23Register(int num, int* m23_data);
	void write_M23Register(int num, int* m23_addr, int* m23_data);
	bool  monopolizeCpu_w(int m_addr, unsigned char *wtbuff, unsigned char num);
	bool monopolizeCpu_r(int m_addr, unsigned char *rdbuff, unsigned char num);

	bool  monopolizeCpu_w_REG64(int m_addr, unsigned char *wtbuff, unsigned char num);
	bool monopolizeCpu_r_REG64(int m_addr, unsigned char *rdbuff, unsigned char num);

	void HardRST(int i );

	void REG64_rd(int subAddr, unsigned char num, unsigned char *rdbuffer);
	void REG64_wt(int subAddr, unsigned char num, unsigned char *wtbuffer);
	unsigned char read_from_external_memory(int addr, unsigned char num, unsigned char *buff);
	unsigned char write_to_external_memory(int addr, unsigned char num, unsigned char *buff);
	unsigned char check_host_free_REG64();
	unsigned char Check_host_status_REG64(unsigned char status);
	unsigned char ext_one_time_wt(int m_addr,
		unsigned char *wtbuff,
		unsigned char num);
	unsigned char ext_ram_write(int ram_addr,
		unsigned char *wtbff,
		int wt_size);
	unsigned char ext_one_time_rd(int m_addr,
		unsigned char *rdbuff,
		unsigned char num);
	unsigned char ext_ram_read(int ram_addr,
		unsigned char *rdbff,
		int rd_size);
	unsigned char target_ext_memory_rd_wt(unsigned char tg_mode,
		int tg_addr,
		unsigned char *buff,
		int tg_size);
	int reRunNetWork();
	int reRtpUdpStreaming();
	int reHdmi();
	//net
	void udp_REG64_rd(int subAddr, unsigned char num, unsigned char *rdbuffer);
	void udp_REG64_wt(int subAddr, unsigned char num, unsigned char *wtbuffer);

	//TS
//	void getTS


#ifdef Q_OS_WIN //windows
	SOCKET  udpOpen(QString ipaddr, int ipport);
	SOCKET  udpClose(SOCKET ufd);
#else
	int udpOpen(QString ipaddr, int ipport);
	int udpClose(int ufd);
#endif
};

#endif // TBSFUNC_H
