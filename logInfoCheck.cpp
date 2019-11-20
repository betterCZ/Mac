#define FILEREADER_EXPORTS
#include"FileReader.h"
std::vector<string> FileReader::m_vFileByLineToArry;
std::unordered_map <string, vector<string> >FileReader::m_mapConfig;
std::vector<string>  FileReader::m_vLogTime;
//std::map<string, set<int>> TextQuery:: m_wordInLine;
std::map<string,vector<int>> globalWordInLine;
std::map<int, std::vector<std::vector<int>>> mFindInfoList;// key=日志类型 value=类型日志出现的行号

void FileReader::ReadFileToarry() //将日志文件读入到 vector中
{   //将日志文件读入到 vector中 并删除类似 [now time: 2019-10-16 09:52:01.058]的头部 将时间读入到单独一个vector中
	std:: string str;
	std::ifstream fin;
	fin.open(m_strFileName);
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			getline(fin,str);
			string sTime = str.substr(0, 35);
			m_vLogTime.push_back(sTime);
			str.erase(0, 35);
			m_vFileByLineToArry.push_back(str);
		}
		std::cout << "已经将文件保存到数组" << endl;
		/*for (int i = 0; i < m_vFileByLineToArry.size(); i++)
		{
			cout << m_vFileByLineToArry[i] << endl;
		}*/
	}
	else
	{
		std::cout << "打开文件失败" << endl;
	}
	//cout << m_vLogTime.size() << endl;
}    //j


vector<vector<string> > FileReader::ReadConfig()//读配置文件，读到一个二维数组中 vector<vector<string> > vvConfig;
{       
	std::string str;
	std::string wordString;
	std::string strLine;
	std::ifstream fin;
	std::vector<string> vecByLine;
	std::vector<vector<string> > vvConfig;
	int firstFlag = 0;
	fin.open(m_strconfigFileName);
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			getline(fin, str);
			for (int i = 0; i < str.size(); i++)
			{
					if (str[i] != ','){
						wordString += str[i];
					}
					else if (',' == str[i] || '\n' == str[i])
					{
						vecByLine.push_back(wordString);
						wordString.clear();
					}
				}
				vecByLine.push_back(wordString);//存入每行最后一个单词
				vvConfig.push_back(vecByLine);
				vecByLine.clear();	
		}
	}
	else
	{
		std::cout << "打开文件失败" << endl;
	}  
	//cout << vvConfig[0][13] << endl;
	
	   return vvConfig;
} 

void FileReader::ReadConfigFileToMap()
{    //配置文件读入到 std::unordered_map < string, vector<string>> mapConfig （key值为属性，value为该属性对应的内容）
	std::string stringLine;
	int theFirstFlag = 0; //存入第一行 
	std::vector<string> theFirstRow;
	std::vector<vector<string> > vvConfig;
	vvConfig = ReadConfig();
	std::vector<string> vecConfigColumn;
	theFirstRow.assign(vvConfig[0].begin(), vvConfig[0].end());
	std::cout << "读取配置文件成功" << endl;
	//cout << vvConfig.size() << endl;
	//cout << Column << endl;
	for (int  i = 0; i < Column; i++)  //控制列
	{    
		for (int j = 1; j < vvConfig.size()-1;j++)
		{ //控制行
			vecConfigColumn.push_back(vvConfig[j][i]);
		}
		m_mapConfig.insert(make_pair(theFirstRow[i],vecConfigColumn));
		vecConfigColumn.clear();
	}
	std::cout << "已经将配置文件读入到map中，形成对应关系" << endl;
	 //map中的对应关系呈现函数
	//std::unordered_map < string, vector<string>> ::iterator it = m_mapConfig.begin();
	//while (it!=m_mapConfig.end())
	//{
	//	cout << it->first << "-------->";
	//	/*for (int i = 0; i < (it->second).size();i++){
	//		cout << (it->second)[i]<<"  ";
	//	}*/
	//	cout << endl;
	//	it++;
	//}
}

std::vector<int> TextQuery::query(const string  &findString)  //根据finsString进行查询 并保存相应查询字符的行号
{                                                             //globalWordInLine[findString]=vector<行号> 为全局变量
	std::string splitFindString;
	std::istringstream ssplit(findString);
	vector<string> vsplitFindString;
	while (ssplit>>splitFindString)   //将要查找的字符进行分片
	{
		vsplitFindString.push_back(splitFindString);
	}
	for (int i = 0; i < FileReader::m_vFileByLineToArry.size(); i++)
	{
		std::istringstream iss(m_vFileByLineToArry[i]);
		std::string word;
		int num=0;
		while (iss>>word)
		{ 
			//cout << word << "      ";
			if (word==vsplitFindString[num]&&num!=vsplitFindString.size()-1)
			{
				num++;
				continue;
			}
			else if (word==vsplitFindString[num]&&num==vsplitFindString.size()-1)
			{
				m_wordInLine[findString].insert(i); 
				break;
			}
			else if (word!=vsplitFindString[num])
			{
				continue;
			}
		}
	}
	std::map<string, set<int>>::iterator it = m_wordInLine.begin();
	/*std::cout << "查询成功" << endl;
	while (it!= m_wordInLine.end())
	{
		cout << it->first << " :";
		for (const int &n : (it->second))
		{
			cout << n<<" ";
		}
		cout << endl;
		it++;
	}*/
	
	vector<int> vFindResult;
	
	while (it!=m_wordInLine.end())
	{
		if (0==m_wordInLine[findString].size())
		{
			break;
		}
		for (const int &n:(it->second))
		{
			vFindResult.push_back(n);
		}
		it++;
	}
	globalWordInLine[findString] = vFindResult;
	m_wordInLine.clear();
	return vFindResult;
}

/*std::map<int, std::vector<std::vector<int>>>*/ void  TextQuery::OrderConfigMap(){
	std::unordered_map <string, vector<string> >::iterator it = FileReader::m_mapConfig.begin();
	std::vector<int> vmapRowNum;
	map<string, set<int>>  mAcceptResult;
	std::unordered_map <string, vector<string> >::iterator itFindString;
	std::unordered_map <string, vector<string> >::iterator itLogType;
	std::unordered_map <string, vector<string> >::iterator itDescripSuc;
	std::unordered_map <string, vector<string> >::iterator itDescripFail;
	std::unordered_map <string, vector<string> >::iterator itResultDisaply;
	//日志类型分类
	//std::map<int, std::vector<std::vector<int>>> mFindInfoList;// key=日志类型 value=类型日志出现的行号
	int n = 0;
	while (n < Column)
	{
		if (3 == n)
		{
			itLogType = it;
		}
		else if (8 == n)
		{
			itFindString = it;
		}
		else if (10 == n)
		{
			itDescripSuc = it;
		}
		else if (11 == n)
		{
			itDescripFail = it;
		}
		else if (12 == n)
		{
			itResultDisaply = it;
		}
		n++;
		it++;
	}

	vector<int> findResultRowNum; //保存了查找到字符串 所出现的所有行号
	int flag; //是否查找到 FindString
	clock_t start, end;
	start = clock();

	for (int i = 0; i < m_mapConfig[itFindString->first].size(); i++)
	{
		//cout << m_mapConfig[itFindString->first][i] << ":";//在map中找到了所有要查询的字符串
		findResultRowNum = TextQuery::query(m_mapConfig[itFindString->first][i]);
		//cout << "一共出现在了" << findResultRowNum.size() << "行" << endl;
		/*for (int i = 0; i < findResultRowNum.size(); i++)
		{
		cout << findResultRowNum[i] << endl;
		}
		cout << "=---------------------------------" << endl;*/
		if (0 == findResultRowNum.size())
		{
			flag = 0;
		}
		else
		{
			flag = 1;
		}
		if (3 == atoi(m_mapConfig[itLogType->first][i].c_str()))
		{
			//cout << "这是一条启动信息" << endl;  //只要出现UserInterfaceApi::Init success 则成功
			/*if (m_vFileByLineToArry[findResultRowNum[0]]=="UserInterfaceApi::Init success"){
				cout << "SDK Initialization successful" << endl;
				}
				else
				{
				cout << "Initialization unknown" << endl;
				}*/
			//vInitInfoShowRow = findResultRowNum;
			mFindInfoList[atoi(m_mapConfig[itLogType->first][i].c_str())].push_back(findResultRowNum);

		}
		else if (2 == atoi(m_mapConfig[itLogType->first][i].c_str()))
		{
			//cout << "这是一条API信息" << endl;
			mFindInfoList[atoi(m_mapConfig[itLogType->first][i].c_str())].push_back(findResultRowNum);
		}
		else if (1 == atoi(m_mapConfig[itLogType->first][i].c_str()))
		{
			//cout << "这是一条设备信息" << endl;
			mFindInfoList[atoi(m_mapConfig[itLogType->first][i].c_str())].push_back(findResultRowNum);
		}
		findResultRowNum.clear();
	}
	end = clock();
	cout << "查询一共耗时:" <<static_cast<double>((end - start)/1000) <<"秒"<< endl;
	/*std::map<int,std::vector<std::vector<int>>> ::iterator itTmp = mFindInfoList.begin();
	while (itTmp != mFindInfoList.end())
	{
		cout << itTmp->first<<":" << endl;
		for (int i = 0; i < itTmp->second.size();i++)
		{
			
			for (int j = 0; j < (itTmp->second[i]).size();j++)
			{
				cout << (itTmp->second)[i][j] << " ";
			}
		}
		itTmp++;
		cout << endl;
	}*/
	//return mFindInfoList;
}

std::string InformationSearch::copyConfigToNewFile(){
	ifstream in;
	ofstream out;
	in.open(m_configName);
	std::string newFileName = "newConfig.txt";
	out.open(newFileName);

	if (!in)
	{
		cerr << "打开失败" << endl;
		return nullptr ;
	}
	if (!out)
	{
		cerr << "打开失败" << endl;
		return nullptr;
	}
	std::string oldLine;
	while (getline(in,oldLine))
	{
		out << oldLine;
		out << "\n";
	}
	out << endl;
	in.close();
	out.close();
	cout << "已经将配置文件复制到新的展示文件" << endl;
	return newFileName;
}

void   InformationSearch::writeResultToNweFile()  //往m_mapConfig中写入结果
{
	std::unordered_map <string, vector<string> >::iterator it = FileReader::m_mapConfig.begin(); //类型到内容的一对多
	std::unordered_map <string, vector<string> >::iterator itFindString;
	std::unordered_map <string, vector<string> >::iterator itLogType;
	std::unordered_map <string, vector<string> >::iterator itDescripSuc;
	std::unordered_map <string, vector<string> >::iterator itDescripFail;
	std::unordered_map <string, vector<string> >::iterator itResultDisaply;
	std::unordered_map <string, vector<string> >::iterator itRowNum;
	std::unordered_map <string, vector<string> >::iterator itLogPrintTime;
	std::map<string, vector<int>> ::iterator itLine = globalWordInLine.begin();

	int n = 0;
	while (n < Column)
	{
		if (3 == n)
		{
			itLogType = it;
		}
		else if (8 == n)
		{
			itFindString = it;
		}
		else if (10 == n)
		{
			itDescripSuc = it;
		}
		else if (11 == n)
		{
			itDescripFail = it;
		}
		else if (12 == n)
		{
			itResultDisaply = it;
		}
		else if (13 == n)
		{
			itRowNum = it;
		}
		else if (14==n)
		{
			itLogPrintTime = it;
		}
		n++;
		it++;
	}
	itLine = globalWordInLine.begin();
	std::vector<string> LogTime;
	//往文件中写时间和行号
	while (itLine != globalWordInLine.end())
	{
		/*cout << itLine->first << ":";
		for (int i = 0; i < itLine->second.size(); i++)
		{
			cout << itLine->second[i] << " ";
		}
		cout << endl;
		*/
		//itLine++;
		for (int i = 0; i < itFindString->second.size();i++)
		{
			if (itLine->first == itFindString->second[i] )
			{
				  std:: string sRow = "";
				for (int j = 0; j < itLine->second.size();j++)
				{
					LogTime.push_back(m_vLogTime[itLine->second[j]]);
					sRow+= std::to_string(itLine->second[j]) + " ";
				}
				//cout << itLine->first << ":";
			   //cout << sTime << endl;
				itRowNum->second[i] = sRow;
				//cout << itRowNum->second[i] << "---------------";
				std:: string sTime = "";
				for (int k = 0;k<LogTime.size();k++)
				{
					sTime += LogTime[k] + "   ";
				}
				itLogPrintTime->second[i] = sTime;
				//cout << itLogPrintTime->second[i] << endl;
				LogTime.clear();
			}

		}
		itLine++;
	}
	
for (int i = 0; i < itFindString->second.size(); i++)
{
		cout << itFindString->second[i] << endl;
		itLine = globalWordInLine.begin();
		while (itLine != globalWordInLine.end())
		{
			//cout << itLine->first << endl;
			if (itFindString->second[i] == itLine->first && itLine->second.size() != 0)
			{
				if (1 != atoi(itLogType->second[i].c_str())) //不是设备信息
				{
					std::string  result = itDescripSuc->second[i];
					itResultDisaply->second[i] = result;
					//cout << result << endl;
					break;
				}
				else if (1 == atoi(itLogType->second[i].c_str()))   //设备信息
				{
					if (itFindString->second[i] == "SDK Init"  && globalWordInLine[itFindString->second[i]].size() != 0)
					{
						int rowNum;
						for (const int & n : globalWordInLine[itFindString->second[i]])
						{
							rowNum = n;
						}
						std::vector<char> vDevice;
						for (int j = 0; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] >= '0'&&m_vFileByLineToArry[rowNum][j] <= '9')
							{
								vDevice.push_back(m_vFileByLineToArry[rowNum][j]);
							}
						}
						std::string s1 = "Device = ";
						std::string  resultStr = s1 + vDevice[2];
						itResultDisaply->second[i] = resultStr;
						break;
						//cout << itResultDisaply->second[i] << endl;
					}
					else if (itFindString->second[i] == "UserInterfaceApi::GetSdkVersion version"  && globalWordInLine[itFindString->second[i]].size() != 0)
					{
						int rowNum;
						for (const int & n : globalWordInLine[itFindString->second[i]])
						{
							rowNum = n;
						}
						std::vector<char> vDevice;
						//cout << m_vFileByLineToArry[rowNum] << endl;
						std::string resultStr = "version  ";
						for (int j = 0; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] == '=')
							{
								for (int k = j; k < m_vFileByLineToArry[rowNum].size(); k++)
								{
									resultStr += m_vFileByLineToArry[rowNum][k];
								}
								break;
							}
						}
						itResultDisaply->second[i] = resultStr;
						break;
						//cout << itResultDisaply->second[i] << endl;
					}
					else if (itFindString->second[i] == "GearVrSDK_connect_camera1"  && globalWordInLine[itFindString->second[i]].size() != 0)
					{
						int rowNum;
						for (const int & n : globalWordInLine[itFindString->second[i]])
						{
							rowNum = n;
						}
						std::string vid = "";
						std::string pid = "";
						int index = 0;
						int j = 0;
						for (j = 0; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] == '=')
							{
								int num = j + 2;
								while (m_vFileByLineToArry[rowNum][num] >= '0'&&m_vFileByLineToArry[rowNum][num] <= '9'){
									vid += m_vFileByLineToArry[rowNum][num];
									num++;
								}
								j = num;
								break;
							}
						}
						for (j; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] == '=')
							{
								int num = j + 2;
								while (m_vFileByLineToArry[rowNum][num] >= '0'&&m_vFileByLineToArry[rowNum][num] <= '9'){
									pid += m_vFileByLineToArry[rowNum][num];
									num++;
								}
								break;
							}
						}
						std::string resultStr = "vid = " + vid + "  pid= " + pid;
						itResultDisaply->second[i] = resultStr;
						break;
						//cout << itResultDisaply->second[i] << endl;
					}
					else if (itFindString->second[i] == "StartCamera begin" && globalWordInLine[itFindString->second[i]].size() != 0)
					{
						int rowNum;
						for (const int & n : globalWordInLine[itFindString->second[i]])
						{
							rowNum = n;
						}
						std::string vid = "";
						std::string pid = "";
						int index = 0;
						int j = 0;
						for (j = 0; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] == '=')
							{
								int num = j + 2;
								while (m_vFileByLineToArry[rowNum][num] >= '0'&&m_vFileByLineToArry[rowNum][num] <= '9'){
									vid += m_vFileByLineToArry[rowNum][num];
									num++;
								}
								j = num;
								break;
							}
						}
						for (j; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] == '=')
							{
								int num = j + 2;
								while (m_vFileByLineToArry[rowNum][num] >= '0'&&m_vFileByLineToArry[rowNum][num] <= '9'){
									pid += m_vFileByLineToArry[rowNum][num];
									num++;
								}
								break;
							}
						}
						std::string resultStr = "vid = " + vid + "  pid= " + pid;
						itResultDisaply->second[i] = resultStr;
					}
					else if (itFindString->second[i] == "GearVrSDK_connect_camera2" && globalWordInLine[itFindString->second[i]].size() != 0)
					{
						int rowNum;
						for (const int & n : globalWordInLine[itFindString->second[i]])
						{
							rowNum = n;
						}
						std::string width = "";
						std::string height = "";
						int count = 0;
						int j = 0;
						for (j = 0; j < m_vFileByLineToArry[rowNum].size(); j++)
						{
							if (m_vFileByLineToArry[rowNum][j] == '=')
							{
								++count;
							}
							if ((2 == count && m_vFileByLineToArry[rowNum][j] == '=') ){
								int num = j + 2;
								while (m_vFileByLineToArry[rowNum][num] >= '0'&&m_vFileByLineToArry[rowNum][num] <= '9'){
									width += m_vFileByLineToArry[rowNum][num];
									num++;
								}
								j = num;
							}
							if ((3 == count && m_vFileByLineToArry[rowNum][j] == '=') )
							{
								int num = j + 2;
								while (m_vFileByLineToArry[rowNum][num] >= '0'&&m_vFileByLineToArry[rowNum][num] <= '9'){
									height += m_vFileByLineToArry[rowNum][num];
									num++;
								}
							}
						}
						std::string resultStr = "width = " + width + " height = " + height;
						itResultDisaply->second[i] = resultStr;
						break;
						//cout << itResultDisaply->second[i] << endl;
					}
				}
			}
			itLine++;
			if (itLine == globalWordInLine.end())
			{
				//cout << "here" << endl;
				itResultDisaply->second[i] = itDescripFail->second[i];
			}
		}
	}
	/*cout << "-----------" << endl;
	for (int i = 0; i < itResultDisaply->second.size(); i++)
	{
		cout << itResultDisaply->second[i] << endl;
	}*/
}

void InformationSearch::interactingSearch()
{
		fflush(stdin);
		std::cout << "-----日志信息查询系统-------" << endl;
		std::cout << "请输入您所需要的查询方式" << endl;
		std::cout << "----------------------" << endl;
		cout << "设备信息            1" << endl;
		cout << "API调用信息         2" << endl;
		cout << "启动信息            3" << endl;
		cout << "结果查询            4" << endl;
		cout << "获取完整查询结果    5 " << endl;
		cout << "请输入您所想要查寻的内容类型：" << endl;
		int typeNum;
		int n = 0;
		auto itM = m_mapConfig.begin();
		std::unordered_map <string, vector<string> >::iterator itId;
		std::unordered_map <string, vector<string> >::iterator itCallTime;
		std::unordered_map <string, vector<string> >::iterator itLogTypeId;
		std::unordered_map <string, vector<string> >::iterator itLog2Type;
		std::unordered_map <string, vector<string> >::iterator itStrlookUpType;
		std::unordered_map <string, vector<string> >::iterator itFindMethod;
		std::unordered_map <string, vector<string> >::iterator itResultDisaply;
		std::unordered_map <string, vector<string> >::iterator itExplain;
		std::unordered_map <string, vector<string> >::iterator itDisplay;
		std::unordered_map <string, vector<string> >::iterator itFindString;
		std::unordered_map <string, vector<string> >::iterator itLogType;
		std::unordered_map <string, vector<string> >::iterator itDescripSuc;
		std::unordered_map <string, vector<string> >::iterator itDescripFail;
		std::unordered_map <string, vector<string> >::iterator itRowNum;
		std::unordered_map <string, vector<string> >::iterator itLogPrintTime;
	
		while (n < Column)
		{
				if (0== n)
				{
					itId = itM;
				}
				else if (1 == n)
				{
					itLog2Type = itM;
				}
				else if (2 == n)
				{
					itFindMethod = itM;
				}
				else if (3 == n)
				{
					itLogType = itM;
				}
				else if (4 == n)
				{
					itExplain = itM;
				}
				else if (5==n)
				{
					itStrlookUpType = itM;
				}
				else if (6==n)
				{
					itCallTime = itM;
				}
				else if (7==n)
				{
					itLogTypeId = itM;
				}
				else if (8==n)
				{
					itFindString = itM; 
				}
				else if (9==n)
				{
					itDisplay = itM;
				}
				else if (10==n)
				{
					itDescripSuc = itM;
				}
				else if (11==n)
				{
					itDescripFail = itM;
				}
				else if (12==n)
				{
					itResultDisaply= itM;
				}
				else if (13==n)
				{
					itRowNum = itM;
				}
				else if (14==n)
				{
					itLogPrintTime = itM;
				}

				n++;
				itM++;
			}
		//cout << itDisplay->first << endl;
		//cout << itFindString->second.size() << endl;
		while (cin >> typeNum)
		{
			if (4 == typeNum)
			{
				for (int i = 0; i < itResultDisaply->second.size(); i++)
				{
					char explainBuf[400];
					char resultBuf[200];
					char rowNumBuf[20];
					char logPrintTimeBuf[200];
					strcpy_s(explainBuf,itExplain->second[i].c_str());
					strcpy_s(resultBuf,itResultDisaply->second[i].c_str());
					strcpy_s(rowNumBuf,itRowNum->second[i].c_str());
					strcpy_s(logPrintTimeBuf,itLogPrintTime->second[i].c_str());

					cout  <<explainBuf<< "                                            "
						<< resultBuf<< "                 "
						<< rowNumBuf << "    "
						<< logPrintTimeBuf<< endl;

				}
				//break;
			}
			//if (5== typeNum)  //遍历整个文件
			//{
			//	for (int i = 0; i < itResultDisaply->second.size();i++)
			//	{
			//	}
			//}
			std::map<int, std::vector<std::vector<int>>>::iterator it = mFindInfoList.begin();
			//cout << mFindInfoList.size() << endl;
			while (it != mFindInfoList.end())
			{
				if (typeNum == it->first)
				{
					
					for (int i = 0; i < it->second.size(); i++)
					{
						for (int j = 0; j < (it->second[i]).size(); j++)
						{
							
							std::cout << m_vFileByLineToArry[it->second[i][j]] << endl;
						}
					}
					break;
				}
				else
				{
					it++;
					continue;
				}
			}
		}
}

void  InitPro (char * logName,char* confName)
{
	if (logName==NULL||confName==NULL)
	{
		return;
	}


	InformationSearch search(logName,confName);
	search.ReadFileToarry();// void
	search.ReadConfigFileToMap();//void
	//search.copyConfigToNewFile();
	search.OrderConfigMap(); //void
	//search.interactingSearch(search.OrderConfigMap());
	search.writeResultToNweFile();  //void
	//search.InfoSearch();
	//search.interactingSearch();
	//char res[200] = { 0 };
	//getInfoBuf("UserInterfaceApi::GetSdkVersion version", res);
	//cout << "ok" << endl;

}


void  getInfoBuf(char* findString, char* res)
{
	if (findString==NULL)
	{
		return;
	}
	int n = 0;
	std::unordered_map <string, vector<string> >::iterator itM = FileReader::m_mapConfig.begin();
	
	std::unordered_map <string, vector<string> >::iterator itResultDisaply;
	std::unordered_map <string, vector<string> >::iterator itFindString;
   /* while (n < Column)
	{
		if (8 == n)
		{
			itFindString = itM;
		}
		if (12 == n)
		{
			itResultDisaply = itM;
			break; 
		}
		itM++;
		n++;  
	}*/
	//cout << itFindString->second.size() << endl;
	for (int i = 0; i < FileReader::m_mapConfig["Find string"].size(); i++)
	{	
		if (strcmp(FileReader::m_mapConfig["Find string"][i].c_str(), findString) == 0)
		{
			strcpy_s(res, strlen(FileReader::m_mapConfig["Display information"][i].c_str()) + 1, FileReader::m_mapConfig["Display information"][i].c_str());
			return;
		}
	}
}


void getAllInfo(char * res)
{
	string result = "";
	for (int i = 0; i < FileReader::m_mapConfig["Display information"].size(); i++)
	{
		string tmp = "";
		string s1 = FileReader::m_mapConfig["Explain"][i] + "    ";
		string s2 = FileReader::m_mapConfig["Display information"][i] + "    ";
		string s3 = FileReader::m_mapConfig["rowNum"][i] + "    ";
		string s4=FileReader::m_mapConfig["LogPrintTime"][i] ;
		tmp = "\r\n"+s1+s2+s3+s4+"\r\n";
		result += tmp;
	}
	strcpy_s(res, strlen(result.c_str()) + 1, result.c_str());
	return;
}

	/*strcpy_s(explainBuf, itExplain->second[i].c_str());
	strcpy_s(resultBuf, itResultDisaply->second[i].c_str());
	strcpy_s(rowNumBuf, itRowNum->second[i].c_str());
	strcpy_s(logPrintTimeBuf, itLogPrintTime->second[i].c_str());*/

	/*cout << explainBuf << "                                            "
		<< resultBuf << "                 "
		<< rowNumBuf << "    "
		<< logPrintTimeBuf << endl;*/

	



