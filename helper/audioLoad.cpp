/**
 * AVStream
 *
 * This is an OpenAL streamer that uses libavformat and  libavcodec to get
 * compressed data and stream them using OpenAL.
 *
 * The bufferFrame function is based on the audio_decode_example by Fabrice
 * Bellard function available at this URL:
 * http://www.libav.org/doxygen/master/libavcodec_2api-example_8c-example.html
 *
 * The audioStream function is based on the OpenAL tutorial available here:
 * http://kcat.strangesoft.net/openal-tutorial.html .
 *
 * Other functions are made by me, using the Doxygen documentation of the
 * used libraries.
 *
 * @author Pier Angelo Vendrame <vogliadifarniente@gmail.com>
 * @license LGPL v2.1 
 */

#include "audioLoad.hpp"

using namespace std;


AVFormatContext *initAvformat()
{
	printf("Initializing Avformat...\n");
	av_register_all();
	AVFormatContext *avfctx = avformat_alloc_context();

	return avfctx;
}

int getAudio(const char *filename, AVFormatContext *avfctx, AVCodec **codec)
{
	printf("Getting information about %s...\n", filename);

	int ret;

	ret = avformat_open_input(&avfctx, filename, NULL, NULL);
	if(ret) {
		printf("avformat_open_input failed and returned %i.\n", ret);
		return 0;
	}

	/*//*/if (avformat_find_stream_info(avfctx, NULL) < 0)printf("Error while calling avformat_find_stream_info");

	if((ret = av_find_best_stream(avfctx, AVMEDIA_TYPE_AUDIO, -1, -1, codec, 0)) >= 0) {
		printf("Audio stream found. It was number %i.\n", ret);
	} else {
		printf("The audio stream wasn't found or there isn't any decoder.\n");
		printf("The function returned %i.\n", ret);
		return 0;
	}

	printf("Codec name: %s (%s);\ncodec id: %i.\n", (*codec)->long_name, (*codec)->name, (*codec)->id);

	return 1;
}

AVCodecContext *initAvcodec(AVCodec *codec)
{
	printf("Initializing Avcodec...\n");
	avcodec_register_all();
	AVCodecContext *avcctx = avcodec_alloc_context3(codec);

	if(avcodec_open2(avcctx, codec, NULL) < 0) {
		printf("Could not open the codec.\n");
		return NULL;
    }

	return avcctx;
}

void shutdownAv(AVFormatContext *avfctx, AVCodec *codec, AVCodecContext *avcctx)
{
	printf("Shutting down AV Libraries...\n");
	avcodec_close(avcctx);
	av_free(avcctx);
	av_free(avfctx);
}

int initOpenAL(ALCdevice **dev, ALCcontext **alctx)
{
	printf("Initializing OpenAL...\n");

	printf("%d %d %d", ALC_FREQUENCY, ALC_REFRESH, ALC_SYNC);

	*dev = alcOpenDevice(NULL);
	if(!(*dev)) {
		printf("Could not open the OpenAL device!\n");
		return 0;
	}

	ALint param[] = {
		44100,
		128,
		4
	};

	*alctx = alcCreateContext(*dev, param);
	alcMakeContextCurrent(*alctx);
	if(!(*alctx)) {
		printf("Could not create the OpenAL context!\n");
		return 0;
	}

	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

	return 1;
}

void shutdownOpenAL(ALCdevice *dev, ALCcontext *alctx)
{
	printf("Shutting down OpenAL...\n");
	alcMakeContextCurrent(NULL);
	alcDestroyContext(alctx);
	alcCloseDevice(dev);
}

void printAlError(ALenum alError)
{
	printf("The error is 0x%X (", alError);
	switch(alError) {
		case AL_INVALID_NAME:
			printf("AL_INVALID_NAME");
		break;

		case AL_INVALID_ENUM:
			printf("AL_INVALID_ENUM");
		break;

		case AL_INVALID_VALUE:
			printf("AL_INVALID_VALUE");
		break;

		case AL_INVALID_OPERATION:
			printf("AL_INVALID_OPERATION");
		break;

		case AL_OUT_OF_MEMORY:
			printf("AL_OUT_OF_MEMORY");
		break;

		default:
			printf("unknown");
	}
	printf(")\n");
}


int bufferFrame(AVCodec *codec, AVCodecContext *avcctx, AVFormatContext *avfctx, vector<unsigned char>& data)
{
	static ALenum format = 0;
	ALenum alError;
	int len;
	int dataSize;
	AVPacket *packet = (AVPacket*)malloc(sizeof(AVPacket));
	AVFrame *decodedFrame = NULL;

	av_init_packet(packet);

	if(av_read_frame(avfctx, packet)) {
		printf("Could not read a frame.\n");
		return -1;
	}

	int gotFrame = 0;

	do {
		if(!decodedFrame) {
			if(!(decodedFrame = avcodec_alloc_frame())) {
				printf("Run out of memory, stop the streaming...\n");
				return -2;
			}
		} else {
			avcodec_get_frame_defaults(decodedFrame);
		}

		len = avcodec_decode_audio4(avcctx, decodedFrame, &gotFrame, packet);
		if(len < 0) {
			printf("Error while decoding.\n");
	    	return -3;
	    }

		if(gotFrame) {
			// We have a buffer ready, send it
			dataSize = av_samples_get_buffer_size(NULL, avcctx->channels,
					decodedFrame->nb_samples, avcctx->sample_fmt, 1);		

			if(!format) {
				if(avcctx->sample_fmt == AV_SAMPLE_FMT_U8 ||
						avcctx->sample_fmt == AV_SAMPLE_FMT_U8P) {
					if(avcctx->channels == 1) {
						format = AL_FORMAT_MONO8;
					} else if(avcctx->channels == 2) {
						format = AL_FORMAT_STEREO8;
					}
				} else if(avcctx->sample_fmt == AV_SAMPLE_FMT_S16 ||
						avcctx->sample_fmt == AV_SAMPLE_FMT_S16P) {
					if(avcctx->channels == 1) {
						format = AL_FORMAT_MONO16;
					} else if(avcctx->channels == 2) {
						format = AL_FORMAT_STEREO16;
					}
				}

				if(!format) {
					printf("OpenAL can't open this format of sound.\n");
					return -4;
				}
			}
			
			//alBufferData(buffer, format, *decodedFrame->data, dataSize, avcctx->sample_rate);
			unsigned char* fdata = static_cast<unsigned char*>(*decodedFrame->data);
			for(int i=0;i<dataSize;i++)
			{
				data.push_back(fdata[i]);
			}
				
			av_free_packet(packet);
			av_free(packet);
			av_free(decodedFrame);

			if((alError = alGetError()) != AL_NO_ERROR) {
				printf("Error while buffering.\n");
				printAlError(alError);
				return -6;
			} else {
				return 1;
			}
		}
	} while(!gotFrame);

	return -5;
}

void getRawData(AVCodec *codec, AVCodecContext *avcctx, AVFormatContext *avfctx, vector<unsigned char>& data)
{
	while(bufferFrame(codec, avcctx, avfctx, data) >= 0);
}

ALenum getFormat(AVCodecContext *avcctx)
{

	if(avcctx->sample_fmt == AV_SAMPLE_FMT_U8 || avcctx->sample_fmt == AV_SAMPLE_FMT_U8P) {
		if(avcctx->channels == 1) {
			return AL_FORMAT_MONO8;
		} else if(avcctx->channels == 2) {
			return AL_FORMAT_STEREO8;
		}
	} else if(avcctx->sample_fmt == AV_SAMPLE_FMT_S16 || avcctx->sample_fmt == AV_SAMPLE_FMT_S16P) {
		if(avcctx->channels == 1) {
			return AL_FORMAT_MONO16;
		} else if(avcctx->channels == 2) {
			return AL_FORMAT_STEREO16;
		}
	}

	
	printf("OpenAL can't open this format of sound.\n");
	return AL_FORMAT_STEREO16;
	
}

int audioStream(AVCodec *codec, AVCodecContext *avcctx, AVFormatContext *avfctx)
{
	printf("Streaming...\n");
	
	
	vector<unsigned char> data;
	ALuint source;
	ALuint buffers[1];
	ALenum format = 0;
	ALuint frequency;
	ALenum alError;

	alGenBuffers(1, buffers);
	alGenSources(1, &source);
	if(alGetError() != AL_NO_ERROR) {
		printf("Could not create buffers or the source.\n");
		return 0;
	}

	int i;
	int dataLength;
	
	getRawData(codec, avcctx, avfctx, data);
	
	//while(bufferFrame(codec, avcctx, avfctx, data) >= 0);
	
	format = getFormat(avcctx);
				
	printf("\n%d\n", (int)data.size());
	alBufferData(buffers[0], format, &data[0], data.size(), avcctx->sample_rate);

	int d;
	scanf("%d",&d);

	//alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
	alSourcei (source, AL_BUFFER,  buffers[0]   );
	
	
	
	alSourcePlay(source);

	if((alError = alGetError()) != AL_NO_ERROR) {
		printf("Error starting the stream.\n");
		printAlError(alError);
		return 0;
	}

	while(1){
	float pos;
	alGetSourcef(source, AL_SEC_OFFSET, &pos );
		printf("%f\n", pos);
		usleep(1000);
	
	}
1;
}

void loadMP3(const char* filename, vector<unsigned char>& data, ALenum& format, ALsizei& sample_rate)
{
	printf("Welcome to the test of AV libraries!\n");


	AVFormatContext *avfctx = initAvformat();
	if(!avfctx) {
		printf("Could not initialize Avformat.\n");
		return;
	}

	AVCodec *codec = (AVCodec*)malloc(sizeof(AVCodec));
	if(!getAudio(filename, avfctx, &codec)) {
		printf("Could not get the audio stream of %s.\n", filename);
		return;
	}

	AVCodecContext *avcctx = initAvcodec(codec);
	if(!avcctx) {
		printf("Could not initialize Avcodec.\n");
		return;
	}

	

	getRawData(codec, avcctx, avfctx, data);
	
	format = getFormat(avcctx);
				
	printf("\n%d\n", (int)data.size());
	//alBufferData(buffers[0], format, &data[0], data.size(), avcctx->sample_rate);
	
	sample_rate = avcctx->sample_rate;

	shutdownAv(avfctx, codec, avcctx);

}
