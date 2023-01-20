// Include Definitions to Build Successfully

#define _CRT_SECURE_NO_WARNINGS


// #include "Sources/InputSource.cpp"
#include "Sources/RawAudioFileSource.cpp"
#include "Sources/RawVideoFileSource.cpp"
#include "Sources/RawAudioDataSource.cpp"
#include "Sources/RawVideoDataSource.cpp"
#include "Sources/EncodedFileSource.cpp"

#include "Frame Sinks/VideoEncoder.cpp"
#include "Frame Sinks/AudioEncoder.cpp"
#include "Frame Sinks/Filter.cpp"

#include "Codecs/AudioCodec.cpp"

#include "Codecs/H265NVEncCodec.cpp"
#include "Codecs/H264NVEncCodec.cpp"
#include "Codecs/VP9Codec.cpp"
#include "Codecs/PNGCodec.cpp"
#include "Codecs/JPGCodec.cpp"
#include "Codecs/Codec.cpp"

#include "FFmpegException.cpp"

#include "Muxing/Muxer.cpp"

#include "Sources/Demuxer.cpp"
#include "SimpleInterface.cpp"