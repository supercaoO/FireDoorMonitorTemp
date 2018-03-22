#include "PrintUtils.h"

string const cutoff_rule(70, '-');

void logging(const string &logging, bool end){
	cout << "ÐÅÏ¢£º" << logging << endl;
	if(end)
		cout << cutoff_rule << endl;
}
void warning(const string &warning, bool end){
	cout << "¾¯¸æ£º" << warning << endl;
	if(end)
		cout << cutoff_rule << endl;
}