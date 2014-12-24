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

#include <stdio.h>
#include <stdint.h>
extern "C" { 
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <AL/al.h>
#include <AL/alc.h>

#include <unistd.h>

#include <vector>


AVFormatContext *initAvformat();

int getAudio(char *filename, AVFormatContext *avfctx, AVCodec **codec);

void shutdownAv(AVFormatContext *avfctx, AVCodec *codec, AVCodecContext *avcctx);

int initOpenAL(ALCdevice **dev, ALCcontext **alctx);

void shutdownOpenAL(ALCdevice *dev, ALCcontext *alctx);

void printAlError(ALenum alError);

int bufferFrame(AVCodec *codec, AVCodecContext *avcctx, AVFormatContext *avfctx, std::vector<unsigned char>& data);

void getRawData(AVCodec *codec, AVCodecContext *avcctx, AVFormatContext *avfctx, std::vector<unsigned char>& data);

ALenum getFormat(AVCodecContext *avcctx);
int audioStream(AVCodec *codec, AVCodecContext *avcctx, AVFormatContext *avfctx);

void loadMP3(const char*, std::vector<unsigned char>&, ALenum&, ALsizei&);
