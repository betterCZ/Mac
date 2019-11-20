#include "FileReader.h"
int main(int argc,char *argv[])
{
	/*FileReading object("file.txt","configfile.txt");
	object.readFileToarry();
	object.readConfigFileToMap();*/
	//FileReading::_init();
	//TextQuery object1("file.txt", "configfile.txt");
	//object1.ReadFileToarry();
	//object1.ReadConfigFileToMap();
	//object1.query("vid");
	//object1.OrderConfigMap();
	//std::map<int, std::vector<std::vector<int>>> tmp = object1.OrderConfigMap();
	//object1.interactingSearch(object1.OrderConfigMap());
	
	InformationSearch search("file.txt", "configfile.txt");
	search.ReadFileToarry();
	search.ReadConfigFileToMap();
	
	search.copyConfigToNewFile();
	std::map<int, std::vector<std::vector<int>>> tmp = search.OrderConfigMap();
	search.writeResultToNweFile();
	//search.InfoSearch();
	search.interactingSearch(tmp);
	system("pause");
}