#ifndef __DATATYPES_H__
#define __DATATYPES_H__
#include <string>
#include <fstream>
#include <vector>

namespace osu
{
	struct String
	{
		unsigned char flag;
		unsigned long long length;
		std::string data;
		
		String();
		void clear();
		int readFrom(std::ifstream &in);
		
	};
	
	struct KeyFrame
	{
		long long w;
		int x;
		float y;
		int z;
	};
	
	class OsuManiaReplayData
	{
	//private:
	public:
		unsigned char mode;
		int version;
		String mapMD5;
		String playerName;
		String replayMD5;
		short n320;
		short n300;
		short n200;
		short n100;
		short n50;
		short n0;
		int score;
		short combo;
		unsigned char fullCombo;
		int mods;
		String lifeBarGraph;
		unsigned long long createTime;
		int replayDataLength;
		unsigned char *replayData;
		unsigned long long replayId;
		double additionalMod;
		
		unsigned char *decodedReplayData;
		std::vector<KeyFrame> frames;
		
	public:
		int readFromPath(std::string path);
		int decode();
		int readFrames();
	};
}
#endif

