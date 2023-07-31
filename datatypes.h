#ifndef __DATATYPES_H__
#define __DATATYPES_H__
#include <string>
#include <fstream>
namespace osu
{
	struct String
	{
		unsigned char flag;
		unsigned long long length;
		std::string data;
		
		int readFrom(ifstream &in);
	};
	
	class OsuManiaReplayData
	{
	private:
		unsigned char mode;
		int version;
		String mapNameMD5;
		String playerName;
		String replayMD5;
		short n320;
		short n300;
		short n200;
		short n100;
		short n50;
		short n0;
		int score;
		int combo;
		bool fullCombo;
		int mods;
		String lifeBarGraph;
		unsigned long long createTime;
		int replayDataLength;
		unsigned char *replayData;
		unsigned long long replayId;
		double additionalMod;
		
	public:
		int readFromPath(std::string path);
	};
}
#endif

