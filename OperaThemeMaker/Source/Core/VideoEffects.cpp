#include "VideoEffects.h"

#include <stdexcept>

namespace OperaThemeMaker
{
	//std::string VideoEffect::GetCommand() const
	//{
	//	throw std::exception("Attempted to call function in parent class, call subclasses instead.");
	//}

	//std::string VideoEffect::GetCommand()
	//{
	//	throw std::exception("Attempted to call function in parent class, call subclasses instead.");
	//}

	std::string BoxBlur::GetCommand() const
	{
		return { "boxblur=" + std::to_string(m_Intensity) };
	}

	std::string BoxBlur::GetCommand()
	{
		return { "boxblur=" + std::to_string(m_Intensity) };
	}

	BoxBlur::operator std::string()
	{
		return "BoxBlur";
	}

	std::string Vignette::GetCommand() const
	{
		return { "vignette=PI/" + std::to_string(m_OverX)};
	}

	std::string Vignette::GetCommand()
	{
		return { "vignette=PI/" + std::to_string(m_OverX) };
	}

	Vignette::operator std::string()
	{
		return "Vignette";
	}

	std::string EdgeDetection::GetCommand() const
	{
		return { "edgedetect" };
	}

	std::string EdgeDetection::GetCommand()
	{
		return { "edgedetect" };
	}

	EdgeDetection::operator std::string()
	{
		return "Edge Detection";
	}


	std::string MonoChrome::GetCommand() const
	{
		return { "hue=s=0" };
	}

	std::string MonoChrome::GetCommand()
	{
		return { "hue=s=0" };
	}

	MonoChrome::operator std::string()
	{
		return "Monochrome";
	}
}
