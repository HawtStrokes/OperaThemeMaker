#pragma once
#include <string>

namespace OperaThemeMaker
{
	class VideoEffect
	{
	public:
		virtual std::string GetCommand() const = 0;
		virtual std::string GetCommand() = 0;
		virtual operator std::string() = 0;
		// VideoEffect() = default;
		virtual ~VideoEffect() = default;
	};

	class BoxBlur final : public VideoEffect
	{
	private:
		unsigned int m_Intensity = 1;
	public:
		[[nodiscard]] std::string GetCommand() const override;
		std::string GetCommand() override;

		void SetIntensity(unsigned int intensity) { m_Intensity = intensity; }
		operator std::basic_string<char>() override;
	};

	class Vignette final : public VideoEffect
	{
	private:
		unsigned int m_OverX = 5;
	public:
		[[nodiscard]] std::string GetCommand() const override;
		std::string GetCommand() override;

		void SetPiDivisor(unsigned int overX) { m_OverX = overX; }
		operator std::basic_string<char>() override;
	};

	class EdgeDetection final : public VideoEffect
	{
	private:
	public:
		[[nodiscard]] std::string GetCommand() const override;
		std::string GetCommand() override;
		operator std::basic_string<char>() override;
	};

	class MonoChrome final : public VideoEffect
	{
	private:
	public:
		[[nodiscard]] std::string GetCommand() const override;
		std::string GetCommand() override;
		operator std::basic_string<char>() override;
	};
}

