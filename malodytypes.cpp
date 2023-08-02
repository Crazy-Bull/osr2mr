#include "malodytypes.h"

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
	
	int MalodyReplayData::transformFrom(const osu::OsuManiaReplayData &omdata, const char *version, unsigned int judge)
	{
		header = "mr format head";
		for(int i = 0; i < 4; i++) 
			this->version[i] = version[i];
		this->mapMD5 = omdata.mapMD5.data;
		
		/* songname, artistname, and judgements cannot be determined */
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
					frames.push_back({(unsigned int)timeStamp, 1, k});
			}
			lastStatus = f[i].x;
		}
		
		frameNum = frames.size();
		
		gameMode = 0; // default: 4key
		
		playTime = omdata.createTime / 10000000 - 62,135,596,800;	// Window Tick to Unix timestamp
	}
}
