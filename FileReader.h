#ifndef FILEREADER_H
#define FILEREADER_H

#ifdef FILEREADER_EXPORTS  
#define FILEREADER_API __declspec(dllexport)  
#else    
#define FILEREADER_API __declspec(dllimport)    
#endif 
extern "C" FILEREADER_API void  getInfoBuf( char* findString, char * res);



//extern "C" FILEREADER_API void  ReadConfigFileToMap();
//extern "C" FILEREADER_API std::vector<vector<string> > ReadConfig();
//extern "C" FILEREADER_API std::vector<int> query(const std::string&findString);
//extern "C" FILEREADER_API std::map<int,std::vector<std::vector<int>>> OrderConfigMap();
//extern "C" FILEREADER_API std::string copyConfigToNewFile();
//extern "C" FILEREADER_API void writeResultToNweFile();
//extern "C" FILEREADER_API void interactingSearch(map<int, std::vector<std::vector<int>>>);
//extern "C" FILEREADER_API void InfoSearch();

#include <set>
#include<iostream>
#include <string>
#include<vector>
#include<fstream>
#include<sstream>
#include<map>
#include<memory>
#include<unordered_map>
#include<regex>
#include<windows.h>
#include<wchar.h>
#include<stdio.h>
#include<stdlib.h>
#include<set>
#include<time.h>
#include <iomanip>
#define Column 15
using namespace std;

	extern std::map<string, vector<int>> globalWordInLine;
	class  FILEREADER_API  FileReader
	{
	private:
		std::string m_strFileName;
		std::string m_strconfigFileName;
	public:
		static std::unordered_map <string, vector<string> > m_mapConfig; //类型到内容的 一对多映射  最终查询比对利用该 map实现
		static std::vector<string> m_vFileByLineToArry;
		static std::vector<string> m_vLogTime;
		/*static bool _init(){
			m_vFileByLineToArry.push_back("string1");
			m_vFileByLineToArry.push_back("string2");
			m_vFileByLineToArry.push_back("string3");
			return true;
			}*/
		FileReader(const string filename, const string configFile)
		{
			m_strFileName = filename;
			m_strconfigFileName = configFile;
		}
		void  ReadFileToarry();      //往数组中读取文件
		void ReadConfigFileToMap();//将配置文件读入到map中
		std::vector<vector<string>> ReadConfig();//将配置文件读入到二维数组中
	};

	class FILEREADER_API TextQuery :public FileReader
	{
	private:
		std::vector<string> m_lines;
		std::map<string, set<int>> m_wordInLine;
	public:
		TextQuery(const std::string filename, const std::string configFile) :FileReader(filename, configFile){};
		std::vector<int> query(const std::string&findString);
		/*std::map<int, std::vector<std::vector<int>>>*/ void  OrderConfigMap();
		//void interactingSearch(map<int,std::vector<std::vector<int>>>);

	};
class FILEREADER_API InformationSearch :public TextQuery
{
	private:
		std::string m_configName;
	public:
		InformationSearch(const std::string filename, const std::string configFile) :TextQuery(filename, configFile)
		{
			m_configName = configFile;
		};
		std::string copyConfigToNewFile();
		void writeResultToNweFile();
		void interactingSearch();
		void InfoSearch();
	};






extern "C" FILEREADER_API void InitPro(char* logName, char* confName);
extern "C" FILEREADER_API void getAllInfo(char *res );

//FILEREADER_API void  getInfoBuf(const  char* findString, char* buf);

#endif







  

	 
