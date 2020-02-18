#include "RdtReceiver.h"
class sr_receiver :public RdtReceiver
{
private:
	int base; //接收窗口基序号
	int N, k; //接收窗口大小，序列长度
	bool *ifrcv; //窗口中每个位置是否有接受的值
	Packet *rcvwinbuf; //接收方窗口缓存
	Packet lastAckPkt;	//上次发送的确认报文

private:
	bool inrcvbuf(int seqnum); //在发送方的滑动窗口中
	void showbuf(); //展示发送方的滑动窗口

public:
	sr_receiver();
	virtual ~sr_receiver();

public:
	void receive(const Packet& packet);	//接收报文，将被NetworkService调用
};