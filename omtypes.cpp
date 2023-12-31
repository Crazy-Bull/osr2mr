#include "omtypes.h"
#include "LzmaDec.h"

#define R(fin, var) (fin).read((char*) &(var), sizeof(var))

namespace osu
{
	/* external allocator for LZMA decoding */
	void *SzAlloc(ISzAllocPtr p, size_t size) { p = p; return malloc(size); }
	void SzFree(ISzAllocPtr p, void *address) { p = p; free(address); }
	ISzAlloc alloc = { SzAlloc, SzFree };
	
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
			R(fin, replayId);
			R(fin, additionalMod);
		}
		else return 1;
		fin.close();
		return 0;
	}

	int OsuManiaReplayData::decode()
	{
		unsigned long long srcLength = replayDataLength - 13;
		unsigned long long destLength = srcLength * 30 + 100;
		unsigned char *dest = new unsigned char[destLength];
		unsigned char *props = replayData;
		ELzmaStatus stat;
		
		SRes res = LzmaDecode(dest, &destLength, replayData + 13, &srcLength, props, 5, LZMA_FINISH_ANY, &stat, &alloc);
		decodedReplayData = (char *)dest;
		return res;
	}
	
	int OsuManiaReplayData::readFrames()
	{
		int ret = 0;
		frames.clear();
		KeyFrame f;
		long long w;
		int x, z, pos, totalPos = 0;
		float y;
		while(4 == sscanf(decodedReplayData + totalPos, "%ld|%d|%f|%d,%n", &w, &x, &y, &z, &pos))
		{
			frames.push_back({w,x,y,z});
			totalPos += pos; 
			ret++;
		}
		return ret;
	}
}
