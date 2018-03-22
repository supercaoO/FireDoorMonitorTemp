#include <iostream>
#include "FireDoor.h"
#include "PrintUtils.h"

using namespace std;

int main(int argc, char* argv[]){
	//日志：程序开启
	string startMsg = "开始检测" + string(argv[1]) + string(" 目录下的 ") + string(argv[13])
				+ string(argv[2]) + string(" 到 ") + string(argv[14]) + string(argv[2]);
	logging(startMsg, true);
	//启动检测
	for(int imgNo = atol(argv[13]); imgNo != atol(argv[14]) + 1; ++imgNo){
		string imgNoStr = to_string((long long) imgNo);
		logging("正在检测 " + imgNoStr + string(argv[2]), false);
		string imgPath = argv[1] + imgNoStr + argv[2];
		FireDoor fireDoor = FireDoor(imgPath, argv);
		if(fireDoor.judgeByBrightness(atof(argv[3]))){
			warning("消防门处于打开状态！！！", true);
			continue;
		} else if(fireDoor.judgeByLines()) {
			warning("消防门处于打开状态！！！", true);
			continue;
		} else if(fireDoor.judgeByCounting(atof(argv[4]))){
			warning("消防门处于打开状态！！！", true);
			continue;
		}
		logging("消防门处于关闭状态", true);
	}
	return 0;
}