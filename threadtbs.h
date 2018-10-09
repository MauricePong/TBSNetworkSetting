#ifndef THREADTBS_H
#define THREADTBS_H
#include <QThread>
#include "tbsfunc.h"
extern volatile bool swflg;
extern volatile u8 mode;

class threadTbs:public QThread
{
    Q_OBJECT
public:
    explicit threadTbs();
    ~threadTbs();
    void writeBuffer();
	void writeIPandPort();
	void lockTable();
	void writeStreamArg();
	/***udpMulticast*******/
	void udpMulticastClinet(void);
signals:
    void sendMsg(Msg* g);
	void sendIp(QString qs, int num);
protected:
    void run();
private:
    tbsfunc et;
    Msg *m;
};

#endif // THREADTBS_H
