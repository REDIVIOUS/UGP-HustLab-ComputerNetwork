#include "RdtSender.h"
class sr_sender :public RdtSender
{
private:
	int base, nextseqnum; //����ţ���һ���������
	int N, k; //���ʹ��ڴ�С�����г���
	bool *ifrcv; //������ÿ��λ���Ƿ��н��ܵ�ֵ
	Packet *sndwinbuf;		//���ͷ����ڻ���

private:
	bool insendbuf(int seqnum); //�ڷ��ͷ��Ļ���������
	void showbuf(); //չʾ���ͷ��Ļ�������

public:

	bool getWaitingState();
	bool send(const Message& message);						//����Ӧ�ò�������Message����NetworkServiceSimulator����,������ͷ��ɹ��ؽ�Message���͵�����㣬����true;�����Ϊ���ͷ����ڵȴ���ȷȷ��״̬���ܾ�����Message���򷵻�false
	void receive(const Packet& ackpkt);						//����ȷ��Ack������NetworkServiceSimulator����	
	void timeoutHandler(int seqnum);					//Timeout handler������NetworkServiceSimulator����

public:
	sr_sender();
	virtual ~sr_sender();
};
