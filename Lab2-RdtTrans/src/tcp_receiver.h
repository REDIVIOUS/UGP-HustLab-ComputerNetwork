#include "RdtReceiver.h"
class tcp_receiver :public RdtReceiver
{
private:
	int expectSequenceNumberRcvd;	// 期待收到的下一个报文序号
	Packet lastAckPkt;				//上次发送的确认报文
	const int k; //序号长度

public:
	tcp_receiver();
	virtual ~tcp_receiver();

public:
	void receive(const Packet& packet);	//接收报文，将被NetworkService调用
};