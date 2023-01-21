#pragma once

#include <filesystem>

#include "HawtLib/File/File.h"
#include "zip_file.hpp"
#include "ffmpegcpp.h"
#include "ffmpeg.h"

#include "VideoEffects.h"

using namespace ffmpegcpp;

namespace OperaThemeMaker
{
	struct Persona
	{
		struct
		{
			std::string name, author;
			unsigned int version{};
		} info;

		struct
		{
			std::string background, title_text_color, title_text_shadow, first_frame_image;
		} startPage;

	};

	typedef std::vector <VideoEffect*> VideoEffects;


	namespace Core
	{
		inline void Archive(const std::filesystem::path& background, const std::filesystem::path& persona, const std::filesystem::path& video,
			const std::string_view zipName, const std::filesystem::path& savePath = "")
		{
			miniz_cpp::zip_file zipFile;
			zipFile.write(video.string(), video.filename().string());
			zipFile.write(background.string(), background.filename().string());
			zipFile.write(persona.string(), persona.filename().string());

			std::filesystem::path finalSavePath = savePath; finalSavePath.append(zipName.data());
			zipFile.save(finalSavePath.string());
		}

		inline void Archive(const std::filesystem::path& background, const std::filesystem::path& persona, const std::string_view zipName,
			const std::filesystem::path& savePath = "")
		{
			
			miniz_cpp::zip_file zipFile;

			zipFile.write(background.string(), background.filename().string());
			zipFile.write(persona.string(), persona.filename().string());

			std::filesystem::path finalSavePath = savePath; finalSavePath.append(zipName.data());
			zipFile.save(finalSavePath.string());
		}

		inline void CreatePersonaIni(const Persona& persona)
		{
			HawtLib::File::IniFile iniFile;

			iniFile.CreateSection("Info");
			iniFile.CreateSection("Start Page");

			// Info Section
			iniFile.AddKeyValue("Info", "name", persona.info.name);
			iniFile.AddKeyValue("Info", "author", persona.info.author);
			iniFile.AddKeyValue("Info", "version", std::to_string(persona.info.version));

			// Start Page Section

			iniFile.AddKeyValue("Start Page", "background", std::filesystem::path(persona.startPage.background).filename().string());
			iniFile.AddKeyValue("Start Page", "position", "center center");
			iniFile.AddKeyValue("Start Page", "title text color", persona.startPage.title_text_color);
			iniFile.AddKeyValue("Start Page", "title text shadow", persona.startPage.title_text_shadow);

			if (!persona.startPage.first_frame_image.empty()) 
				iniFile.AddKeyValue("Start Page", "first frame image", std::filesystem::path(persona.startPage.first_frame_image).filename().string());

			iniFile.Save("output/persona.ini");
		}

		/*inline std::filesystem::path ProcessVideo(const std::filesystem::path& inVideo)
		{
			auto outVideo = inVideo;
			outVideo.replace_extension(".webm");
			std::filesystem::path outputFile = std::filesystem::current_path().append("output").append(outVideo.filename().string());
			void* handle = ffmpegCppCreate((outputFile.string().c_str()));
			ffmpegCppAddVideoStream(handle, inVideo.string().c_str());
			ffmpegCppGenerate(handle);
			ffmpegCppClose(handle);
			return outputFile;
		}*/

		inline std::filesystem::path ProcessVideo(const std::filesystem::path& inVideo,
		                                          const VideoEffects& videoEffects)
		{
			auto outVideo = inVideo;
			outVideo.replace_extension(".webm");
			std::filesystem::path outputFile = std::filesystem::current_path().append("output").append(outVideo.filename().string());
			std::string filterString;
			std::vector<VideoEffect*> finalVideoEffect;

			for (auto* videoEffect : videoEffects)
			{
				if (videoEffect != nullptr)
				{
					finalVideoEffect.push_back(videoEffect);
				}
			}

			for (size_t i = 0; i < finalVideoEffect.size(); ++i)
			{
				// prepend source if not first in list of effects
				if (i != 0)
				{
					filterString += "[video]";
				}

				// command + append output if not last in list of effects
				if (i == finalVideoEffect.size() - 1)
				{
					
					filterString += finalVideoEffect[i]->GetCommand();
				}
				else
				{
					filterString += finalVideoEffect[i]->GetCommand() + "[video];";
				}
			}

			// Get input Video Duration
			AVFormatContext* pFormatCtx = avformat_alloc_context();
			avformat_open_input(&pFormatCtx, inVideo.string().c_str(), NULL, NULL);
			std::string bitRate = std::to_string(360000 / (pFormatCtx->duration / AV_TIME_BASE)) + "K";
			avformat_close_input(&pFormatCtx);
			avformat_free_context(pFormatCtx);

			Muxer* muxer = new Muxer(outputFile.string().c_str());
			VideoCodec* codec = new VideoCodec(AV_CODEC_ID_VP9);
			codec->SetQualityScale(0);
			codec->SetGenericOption("b", bitRate.c_str());
			VideoEncoder* encoder = new VideoEncoder(codec, muxer);


			Filter* filter = nullptr;
			if (!filterString.empty())
				filter = new Filter(filterString.c_str(), encoder);

			Demuxer* demuxer = new Demuxer(inVideo.string().c_str());

			if (!filterString.empty())
				demuxer->DecodeBestVideoStream(filter);
			else
				demuxer->DecodeBestVideoStream(encoder);

			demuxer->PreparePipeline();

			while (!demuxer->IsDone())
			{
				demuxer->Step();
			}

			muxer->Close();
			return outputFile;
		}

	}
}

