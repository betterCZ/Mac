#include "FileReader.h"

int main(int argc, char *argv[])
{
	char* log = "C:/Users/7invensun/Documents/Visual Studio 2013/Projects/logCheck/check/log.txt";
	char * conf = "C:/Users/7invensun/Documents/Visual Studio 2013/Projects/logCheck/check/configfile.txt";
	InitPro(log, conf);
	char *res=new char [1024];
	getAllInfo(res);

	//InformationSearch search("log.txt", "configfile.txt");
	// search.ReadFileToarry();// void
 //    search.ReadConfigFileToMap();//void
	// //search.copyConfigToNewFile();
	// search.OrderConfigMap(); //void
	// //search.interactingSearch(search.OrderConfigMap());
	//  search.writeResultToNweFile();  //void
	////search.InfoSearch();
	//search.interactingSearch();
			 /* char res[200] = { 0 };
			  getInfoBuf("UserInterfaceApi::GetSdkVersion version",res);
			  cout << res << endl;*/
	system("pause");
}