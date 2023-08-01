#include "datatypes.h"

#define R(fin, var) (fin).read((char*) &(var), sizeof(var))

namespace osu
{
	String::String()
	{
		clear();
	}
	
	void String::clear()
	{
		this->flag = 0;
		this->length = 0;
		this->data = "";
	}
	
	int String::readFrom(std::ifstream &in)
	{
		R(in, flag);
		if(flag == 0)
		{
			clear();
		}
		else if(flag == 0x0B)
		{
			//read length
			unsigned long long len = 0;
    		int shift = 0;
    		bool flag;
    		unsigned char c;
    		do 
			{
        		R(in, c);
       			len |= (c & 0x7f) << shift;
      			shift += 7;
      			flag = c & 0x80;
    		} while (flag);
    		
    		//read string data
    		char *strbuff = new char[len + 1];
    		in.read(strbuff, len);
    		strbuff[len] = '\0';
    		data = strbuff;
    		delete[] strbuff;
		}
		else
		{
			return 1;
		}
		return 0;
	}
	
	int OsuManiaReplayData::readFromPath(std::string path)
	{
		std::ifstream fin(path, std::ios::in | std::ios::binary);
		std::ofstream fout("test.out", std::ios::out);
		if(fin.is_open())
		{
			R(fin, mode);
			R(fin, version);
			if(mapMD5.readFrom(fin)) return 1;
			if(playerName.readFrom(fin)) return 1;
			if(replayMD5.readFrom(fin)) return 1;
			R(fin, n300);
			R(fin, n100);
			R(fin, n50);
			R(fin, n320);
			R(fin, n200);
			R(fin, n0);
			R(fin, score);
			R(fin, combo);
			R(fin, fullCombo);
			R(fin, mods);
			if(lifeBarGraph.readFrom(fin)) return 1;
			R(fin, createTime);
			R(fin, replayDataLength);
			replayData = new unsigned char[replayDataLength];
			fin.read((char*)replayData, replayDataLength);
			R(fin, additionalMod);
		}
		else return 1;
		fin.close();
		return 0;
	}
}
