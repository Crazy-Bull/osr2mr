#include "omtypes.h"
#include "malodytypes.h"
#include <unistd.h>
#include <iostream>

int main(int argc, char **argv)
{
	char keys[10] = "", version[20] = "", judge[10] = "";
	unsigned int keyNum = 0, judgeNum = 0;
	char judgeChar = 0;
	unsigned int verNum[4] = {0};
	unsigned char ver[4] = {0};
	
	std::string osrPath;
	if(argc == 2)
		osrPath = argv[1];
	
	/* read settings */
	std::string settingsPath = argv[0];
	int i = settingsPath.length();
	while(i >= 0 && settingsPath[i - 1] != '\\') i--;
	settingsPath.resize(i, 0);
	settingsPath.append(".settings", 10);
	
	std::ifstream fset(settingsPath, std::ios::in);
	if(fset.is_open())
	{
	 	fset.getline(keys, 10);
	 	fset.getline(version, 20);
	 	fset.getline(judge, 10);
	 	sscanf(keys, "keys=%d", &keyNum);
	 	sscanf(version, "version=%d.%d.%d", verNum + 2, verNum + 1, verNum);
	 	sscanf(judge, "judge=%c", &judgeChar);
	 	fset.close();
	}
	
	/* read args */
	
	char c;
	while ((c = getopt(argc, argv, "k:v:j:f:")) != EOF) 
	{
        switch (c) 
		{
        case 'k':
            keyNum = atoi(optarg);
	    	break;
        case 'v':
            sscanf(optarg, "%d.%d.%d", verNum + 2, verNum + 1, verNum);
	    	break;
        case 'j':
            judgeChar = optarg[0];
	    	break;
	    case 'f':
	    	osrPath = optarg;
	    	break;
		}
	}
	
	if(keyNum == 0 || keyNum >= 10)
	{
		std::cout<<"Argument keys is not valid. Program exits."<<std::endl;
		system("pause");
		return 0;
	}
	
	if(judgeChar >= '0' && judgeChar <= '4')
	{
		judgeNum = judgeChar - '0';
	}
	else if(judgeChar >= 'A' && judgeChar <= 'E')
	{
		judgeNum = judgeChar - 'A';
	}
	else if(judgeChar >= 'a' && judgeChar <= 'e')
	{
		judgeNum = judgeChar - 'e';
	}
	else
	{
		std::cout<<"Argument judge is not valid. Program exits."<<std::endl;
		system("pause");
		return 0;
	}
	
	for(int j = 0; j < 4; j++)
		ver[j] = (unsigned char) verNum[j];
	
	/* prompt messages */
	
	std::cout<<"Argument List:"<<std::endl;
	std::cout<<"Keys:"<<keyNum<<std::endl;
	std::cout<<"Malody Version:"<<verNum[2]<<"."<<verNum[1]<<"."<<verNum[0]<<std::endl;
	std::cout<<"Judge:"<<judgeChar<<std::endl;
	
	/* read, transform and write */
	
	osu::OsuManiaReplayData omdat;
	omdat.readFromPath(osrPath);
	omdat.decode();
	omdat.readFrames();
	
	malody::MalodyReplayData mlddat;
	mlddat.transformFrom(omdat, ver, judgeNum, keyNum);
	
	
	std::string outputPath = settingsPath;
	outputPath.resize(i, 0);
	outputPath.append("output.mr", 10);
	mlddat.writeTo(outputPath);
	
	std::cout<<"Malody replay file has been generated in the folder as output.mr"<<std::endl;
	system("pause");
	return 0;
}
