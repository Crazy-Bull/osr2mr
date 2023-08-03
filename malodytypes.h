#ifndef __MLDTYPES_H__
#define __MLDTYPES_H__
#include "omtypes.h"
#include <string>
#include <fstream>
#include <vector>

namespace malody
{
	struct String
	{
		unsigned int length;
		std::string data;
		String();
		String(std::string str);
		int writeTo(std::ofstream &fout);
		void operator= (std::string str);
	};
	
	struct KeyFrame
	{
		unsigned int timeStamp;
		unsigned char isPress;	// 1: Press, 2: Release
		unsigned char keyNo;	// k: k-th key from the left, leftmost is 0
	};
	
	class MalodyReplayData
	{
	//private:
	public:
		String header;
		unsigned char version[4];
		String mapMD5;
		String difficultyName;
		String songName;
		String artistName;
		unsigned int score;
		unsigned int combo;
		unsigned int best;
		unsigned int cool;
		unsigned int good;
		unsigned int miss;
		unsigned int unknown0;
		unsigned int mods;
		unsigned int judge;	// 0-4: A-E
		
		String dataHeader;
		//unsigned char version[4]; another version code
		unsigned int frameNum;
		unsigned char gameMode;	// 00: 4Key
		unsigned int playTime;
		unsigned int unknown1;
		std::vector<KeyFrame> frames;
		
	public:
		int transformFrom(const osu::OsuManiaReplayData &omdata, const unsigned char *version, unsigned int judge);
		int writeTo(std::string path); 
	};
}


#endif
