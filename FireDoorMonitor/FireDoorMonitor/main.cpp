#include <iostream>
#include "FireDoor.h"
#include "PrintUtils.h"

using namespace std;

int main(int argc, char* argv[]){
	//��־��������
	string startMsg = "��ʼ���" + string(argv[1]) + string(" Ŀ¼�µ� ") + string(argv[13])
				+ string(argv[2]) + string(" �� ") + string(argv[14]) + string(argv[2]);
	logging(startMsg, true);
	//�������
	for(int imgNo = atol(argv[13]); imgNo != atol(argv[14]) + 1; ++imgNo){
		string imgNoStr = to_string((long long) imgNo);
		logging("���ڼ�� " + imgNoStr + string(argv[2]), false);
		string imgPath = argv[1] + imgNoStr + argv[2];
		FireDoor fireDoor = FireDoor(imgPath, argv);
		if(fireDoor.judgeByBrightness(atof(argv[3]))){
			warning("�����Ŵ��ڴ�״̬������", true);
			continue;
		} else if(fireDoor.judgeByLines()) {
			warning("�����Ŵ��ڴ�״̬������", true);
			continue;
		} else if(fireDoor.judgeByCounting(atof(argv[4]))){
			warning("�����Ŵ��ڴ�״̬������", true);
			continue;
		}
		logging("�����Ŵ��ڹر�״̬", true);
	}
	return 0;
}