#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <helper/audioLoad.hpp>

#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <iostream>

class GlobalInfo;

static const char* getALErrorString(int err){
  switch (err) {
    case AL_NO_ERROR:
      return "AL_NO_ERROR";
    case AL_INVALID_NAME:
      return "AL_INVALID_NAME";
    case AL_INVALID_ENUM:
      return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE:
      return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION:
      return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY:
      return "AL_OUT_OF_MEMORY";
    default:
      return "No such error code";
  }
};


class MusicTrack
{
	friend class MusicPlayer;
	protected:
		ALuint buffer;
		
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;
	
	public:
		MusicTrack(const char* text);
		
		~MusicTrack()
		{
			 alDeleteBuffers(1, &buffer);
		}
		 
};

class MusicDB
{
		
};

class MusicPlayer
{
	
	GlobalInfo& globalInfo;
	
	ALCdevice *dev;
    ALCcontext *ctx;
    
    ALuint source;
    
	ALfloat SourcePos[3] = { 0.0, 0.0, 0.0 };                                    //Position of the source sound
    ALfloat SourceVel[3] = { 0.0, 0.0, 0.0 };                                    //Velocity of the source sound
    ALfloat ListenerPos[3] = { 0.0, 0.0, 0.0 };                                  //Position of the listener
    ALfloat ListenerVel[3] = { 0.0, 0.0, 0.0 };                                  //Velocity of the listener
    ALfloat ListenerOri[6] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };                 //Orientation of the listener
    
    float prevPlayTime;		//!< Play position of the previous frame
    
    public:
	
		MusicPlayer(GlobalInfo&);
		
		void playTrack(MusicTrack*);
		
		~MusicPlayer()
		{
			
			alDeleteSources(1, &source);
			alutExit();
		}
		
		float getTime()
		{
			float sec;
			alGetSourcef(source, AL_SEC_OFFSET, &sec);
			return sec;
		}
		
		float getChangedTime()
		{
			float sec;
			alGetSourcef(source, AL_SEC_OFFSET, &sec);
			//std::cout << "m " << sec << std::endl;
			if(sec!=prevPlayTime)
			{
				prevPlayTime = sec;
				return sec;
			}
			return 0.f;
		}
	
};

#endif
