#pragma once

#include <string>

namespace OperaThemeMaker
{
	namespace Util
	{
		inline std::string U32ToHex(unsigned int n)
		{
			std::string returnString;

			while (n != 0)
			{
				const unsigned int remainder = n % 16;
				const signed char ch = (remainder < 10) ? static_cast<signed char>(remainder + 48) : static_cast<signed char>(remainder + 55);
				returnString += ch;
				n = n / 16;
			}

			for (size_t i = returnString.size(); i != 6; ++i)
				returnString += "0";

			for (size_t i = 0; i < returnString.size(); ++i)
				std::swap(returnString[i], returnString[returnString.size() - 1 - i]);

			return returnString;
		}
		
	}

}
