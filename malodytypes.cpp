#include "malodytypes.h"

#define W(fout, var) (fout).write((const char*) &(var), sizeof(var))
namespace malody
{
	String::String()
	{
		length = 0;
		data = "";
	}
	
	String::String(std::string str)
	{
		data = str;
		length = str.length();
	}
	
	int String::writeTo(std::ofstream &fout)
	{
		W(fout, length);
		fout<<data;
	}
	
	void String::operator= (std::string str)
	{
		data = str;
		length = str.length();
	}
	
	int MalodyReplayData::transformFrom(const osu::OsuManiaReplayData &omdata, const unsigned char *version, unsigned int judge)
	{
		header = "mr format head";
		for(int i = 0; i < 4; i++) 
			this->version[i] = version[i];
		mapMD5 = omdata.mapMD5.data;
		
		/* songname, artistname, and judgements cannot be determined */
		difficultyName = "";
		songName = "";
		artistName = "";
		best = cool = good = miss = 0;
		unknown0 = 0;
		
		/* mods */
		mods = 0;
		mods |= ((omdata.mods >> 30) & 1) << 2;	// MR
		mods |= ((omdata.mods >> 6) & 1) << 5;	// DT | NC
		this->judge = judge;
		
		dataHeader = "mr data";
		
		/* frame transform */
		auto &f = omdata.frames;
		long long timeStamp = f[0].w;
		int lastStatus = f[0].x;
		int n = f.size();
		for(int i = 1; i < n; i++)
		{
			timeStamp += f[i].w;
			for(unsigned char k = 0; k < 4; k++)
			{
				/* key press */
				if(((f[i].x >> k) & 1) && !((lastStatus >> k) & 1))	
					frames.push_back({(unsigned int)timeStamp, 1, k});
				
				/* key release */
				if(!((f[i].x >> k) & 1) && ((lastStatus >> k) & 1))	
					frames.push_back({(unsigned int)timeStamp, 2, k});
			}
			lastStatus = f[i].x;
		}
		
		frameNum = frames.size();
		
		gameMode = 0; // default: 4key
		
		playTime = omdata.createTime / 10000000 - 62135596800LL;	// Window Tick to Unix timestamp
		
		return 0;
	}
	
	
	
	int MalodyReplayData::writeTo(std::string path)
	{
		std::ofstream fout(path, std::ios::out | std::ios::binary);
		
		header.writeTo(fout);
		
		for(int i = 0; i < 4; i++) 	W(fout, version[i]);
		mapMD5.writeTo(fout);
		difficultyName.writeTo(fout);
		songName.writeTo(fout);
		artistName.writeTo(fout);
		W(fout, score);
		W(fout, combo);
		W(fout, best);
		W(fout, cool);
		W(fout, good);
		W(fout, miss);
		W(fout, unknown0);
		
		W(fout, mods);
		W(fout, judge);
		
		dataHeader.writeTo(fout);
		
		for(int i = 0; i < 4; i++) W(fout, version[i]);
		W(fout, frameNum);
		W(fout, gameMode);
		W(fout, playTime);
		W(fout, unknown1);
		
		for(int i = 0; i < frameNum; i++)
		{
			W(fout, frames[i].timeStamp);
			W(fout, frames[i].isPress);
			W(fout, frames[i].keyNo);
		}
		
		return 0;
	}
}
