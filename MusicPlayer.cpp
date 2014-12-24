

#include "MusicPlayer.hpp"
#include <infoContainer/GlobalInfo.hpp>
#include <SoundTouch/SoundTouch.h>
#include <SoundTouch/STTypes.h>
#include <SoundTouch/BPMDetect.h>

using namespace soundtouch;

#define BUFF_SIZE (1024*1024*10)

MusicTrack::MusicTrack(const char* file)
{
	
	alGenBuffers(1, &buffer);
	std::cout << "1 " << getALErrorString(alGetError()) << std::endl;
	
	vector<unsigned char> data;
	ALenum format;
	ALsizei freq;
	
	loadMP3(file, data, format, freq);
	
	SoundTouch* pSoundTouch = new SoundTouch;
	//pSoundTouch->setSetting(SEQUENCE_MS, 1);
	//pSoundTouch->setSetting(SEEKWINDOW_MS, 1);
	//pSoundTouch->setSetting(OVERLAP_MS, 1);
	pSoundTouch->setRate(1);
	pSoundTouch->setTempo(1.5);
	pSoundTouch->setPitch(1);
	
	pSoundTouch->setSampleRate(freq);
	pSoundTouch->setChannels(2);
	
	int length = 1000000;
	
	cout << data.size() << endl;
	
	vector<char> data2;
	data2.resize(1000);
	
	/*for(int i=0;i<data.size();i++)
	{
		if((i+1)%16==0)data[i] = 0;
	}*/
	cout << "CMP: " << memcmp(&data2[0], &data[0], 1000) << endl;
	
	
	//pSoundTouch->flush();
	
	//char* ch = new char[data.size()];
	
	//memcpy(ch, &data[0], data.size());
	
	//BPMDetect* bpm = new BPMDetect(2, freq);
	//const SAMPLETYPE* c = (const SAMPLETYPE*)&data[0];
	//cout << ch << endl;
	//bpm->inputSamples((SAMPLETYPE*)ch, data.size()/2);

	//cout << "BPM " << bpm->getBpm() << endl;
	//exit(0);
	

	int nChannels = 2;
	int nSamples;
	int cumSamples=0;
	int maxSamples = BUFF_SIZE;
	SAMPLETYPE* sampleBuffer = new SAMPLETYPE[BUFF_SIZE];
	memset(sampleBuffer, 0xfa, BUFF_SIZE*2);
	SAMPLETYPE* sampleStart = sampleBuffer;
	
	//cout << "UPS: " << pSoundTouch->numUnprocessedSamples() << endl;
	//do
	//{
	//	int buffSizeSamples = BUFF_SIZE / nChannels;
		//cout << "NS: " << pSoundTouch->numSamples() << endl;
	
	/*for(int j=0;j<40;j++)
	{
		pSoundTouch->putSamples((const SAMPLETYPE*) &data[0] + j*freq*8, freq*4);
		nSamples = pSoundTouch->receiveSamples(sampleStart, maxSamples);
		maxSamples-=nSamples;
		cumSamples+=nSamples;
		sampleStart+=nSamples*2;
		
		cout << "UPS: " << pSoundTouch->numUnprocessedSamples() << endl;
		cout << "NS: " << pSoundTouch->numSamples() << " " << nSamples << " " << freq/1.5*4 << endl;
		cout <<  "Recv sample " << j << endl;
	}*/
	
	
		//UseReadySound(sampleBuffer, nSamples * nChannels);
	//} while (nSamples != 0 && maxSamples>0);
	
	cout << "Totals " << cumSamples << " " << maxSamples << endl;

	cout << "S: " << sizeof(SAMPLETYPE) << endl;

	for(int i=0;i<100;i++)
	{
		//sampleBuffer[i] = data[i*2] + data[2*i+1]*256;
		cout << std::hex << short(sampleBuffer[i]) << " " << int(data[i*2]) << " " << int(data[(2*i)+1]) << endl;
	}
	
	cout << std::dec;
	
	
	
	
	std::cout << "2 " << getALErrorString(alGetError()) << std::endl;
	alBufferData(buffer, format, &data[0], data.size(), freq);
	//alBufferData(buffer, format, sampleBuffer, BUFF_SIZE, freq);
	std::cout << "3 " << getALErrorString(alGetError()) << std::endl;
	std::cout << buffer << " " << format << " " << size << " " << freq << std::endl;
	
	std::cout << "4 " << getALErrorString(alGetError()) << std::endl;
}

MusicPlayer::MusicPlayer(GlobalInfo& gi):
globalInfo(gi)
{
	/*dev = alcOpenDevice(NULL);
	if(!dev)
	{
		fprintf(stderr, "Could not open AL device\n");
		exit(1);
	}
	ctx = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(ctx);
	if(!ctx)
	{
		fprintf(stderr, "Could not create AL context\n");
		exit(1);
	}*/
	cout << "Set up AL" << endl;
	dev = alcOpenDevice(NULL);
	ctx = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(ctx);
	
	unsigned int a = alGetError();
	if (a!=AL_NO_ERROR) cout << "AL_Error: " << a << endl;

	alGenSources(1, &source);
	a = alGetError();
	if (a!=AL_NO_ERROR) cout << "AL_Error: " << a << endl;
	
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
                                                                                //First direction vector, then vector pointing up) 
    //Listener                                                                               
    alListenerfv(AL_POSITION,    ListenerPos);                                  //Set position of the listener
    alListenerfv(AL_VELOCITY,    ListenerVel);                                  //Set velocity of the listener
    alListenerfv(AL_ORIENTATION, ListenerOri);   
    
    
    
}

void MusicPlayer::playTrack(MusicTrack* mt)
{
	cout << "Playing track" << endl;
	alSourcef(source, AL_PITCH, 1.f);
	alSourcei (source, AL_BUFFER,   mt->buffer   );
	alSourcef(source, AL_SEC_OFFSET, 20.);
	alSourcef(source, AL_GAIN, 0.1f);
	alSourcePlay(source);
	glfwSetTime(0);
	prevPlayTime = 0;
	unsigned int a = alGetError();
	if (a!=AL_NO_ERROR) cout << "AL_Error: " << a << endl;
}
