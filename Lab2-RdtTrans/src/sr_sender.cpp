#include "stdafx.h"
#include "Tool.h"
#include "Global.h"
#include "sr_sender.h"
using namespace std;

sr_sender::sr_sender() :k(8), N(4), ifrcv(new bool[8]), sndwinbuf(new Packet[8])
{
	base = 0;
	nextseqnum = 0;
	//清空窗口所有状态为未接受
	for (int i = 0; i < k; i++) {
		ifrcv[i] = false;
	}
}

sr_sender::~sr_sender()
{
	delete[] sndwinbuf;
	delete[] ifrcv;
}

bool sr_sender::send(const Message& message)
{
	if (getWaitingState()) {
		pUtils->printPacket("发送方：发送窗口满，无法发送", sndwinbuf[nextseqnum]);
		return false;
	}
	else if (insendbuf(nextseqnum)){ //如果要发送的包在窗口内，发送
		//处理下一个要发的分组
		sndwinbuf[nextseqnum].acknum = -1; //忽略该字段
		sndwinbuf[nextseqnum].seqnum = nextseqnum; //下一个要发送的序列号；
		//sndwinbuf[nextseqnum].checksum = 0; //初始化checksum为0
		memcpy(sndwinbuf[nextseqnum].payload, message.data, sizeof(message.data)); //data
		sndwinbuf[nextseqnum].checksum = pUtils->calculateCheckSum(sndwinbuf[nextseqnum]); //计算checksum
		pUtils->printPacket("发送方：发送方发送报文", sndwinbuf[nextseqnum]);
		//发送报文前的发送方窗口
		cout << "发送方：发送方发送前的滑动窗口：\n";
		showbuf();
		//为每一个分组，启动定时器
		pns->startTimer(SENDER, Configuration::TIME_OUT, nextseqnum);
		//发送给接收方
		pns->sendToNetworkLayer(RECEIVER, sndwinbuf[nextseqnum]);
		nextseqnum = (nextseqnum + 1) % k; //准备发送下一个报文
		//nextseqnum = (nextseqnum + 1) % N; //准备发送下一个报文
		//发送后的滑动窗口
		cout << "发送方：发送方发送后的滑动窗口：\n";
		showbuf();
		return true;
	}
	else{ //否则忽略处理
		return false;
	}
}

void sr_sender::receive(const Packet& ackpkt)
{
	//比较校验和，检查分组是否被损坏
	int checksum;
	checksum = pUtils->calculateCheckSum(ackpkt);
	if (checksum != ackpkt.checksum) {
		pUtils->printPacket("发送方：校验和不正确，分组损坏", ackpkt);
		return;
	}
	else { //否则准备接收返回的报文
		if (insendbuf(ackpkt.acknum)) {
			cout << "发送方：接收到ACK：" << ackpkt.acknum << endl;
			pns->stopTimer(SENDER, ackpkt.acknum); //停止计时
			ifrcv[ackpkt.acknum] = true; //将当前窗口置为已被接受
			while (ifrcv[base] == true) { //循环滑动窗口，到没有发送为止
				ifrcv[base] = false; //释放该窗口
				base = (base + 1) % k; //向后移动窗口
			}
			//接收到ACK后滑动窗口移动
			cout << "发送方：收到接收方返回的ACK,滑动窗口变为：\n";
			showbuf();
		}
	}
}

//是否应该等待
bool sr_sender::getWaitingState() //true为窗口满（要等待）
{
	int limitseq = base + N;
	int crntcap = limitseq % k; //目前窗口容量到达序号
	int crntocup = nextseqnum % k; //目前占用到的序号
	bool tail = (crntcap == crntocup); //判断当前的序号是否已经到窗口的末尾
	return tail ;
}

//超时了之后，重传该分组
void sr_sender::timeoutHandler(int seqnum)
{
	//有数据包超时了立即重传
	pUtils->printPacket("发送方：有分组发生超时", sndwinbuf[seqnum]);
	pns->sendToNetworkLayer(RECEIVER, sndwinbuf[seqnum]);
	pns->stopTimer(SENDER, seqnum); //对seqnum重启计时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum); //对seqnum重启计时器
	pUtils->printPacket("发送方：重传完成", sndwinbuf[base]);
}

//判断当前分组是否在发送方窗口中
bool sr_sender::insendbuf(int seqnum)
{
	int end = (base + N) % k;
	//分两种情况，首先是发送方窗口末尾的序列号大于起始序列号，证明升序排列
	if (base < end) {
		//该分组序列号seqnum要限制在发送窗口的首尾序号之间
		return seqnum < end && seqnum >= base;
	}
	else {
		//窗口末尾序列号小于起始序列号，
		//证明其排列是从开始序列号到k-1,然后再从0到末尾，我们将其限制在这个范围
		return seqnum >= base || seqnum < end;
	}
}

//打印发送窗口
void sr_sender::showbuf()
{
	for (int i = 0; i < k; i++) { //碰到窗口开头输出括号
		if (i == base) cout << "(";
		//在发送缓冲区内的元素
		if (insendbuf(i) && i >= nextseqnum) {
			cout << i << ":empty"; //窗口内，空状态
		}
		else if (insendbuf(i) && ifrcv[i] == false) { //窗口内，已发送没ack状态
			cout << i << ":sent";
		}
		else if (insendbuf(i) && ifrcv[i] == true) { //ack状态
			cout << i << ":acked";
		}
		if (i == (base + N) % k) cout<<")"; //碰到窗口结尾输出反括号
		if (insendbuf(i) == false) cout << i << ":X"; //不在窗口内的元素
		cout << " ";
	}
	cout << "\n";
}