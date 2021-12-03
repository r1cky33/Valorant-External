#include "stdafx.h"

namespace crypt {
	std::string hex2bin(std::string const& s) {
		assert(s.length() % 2 == 0);

		std::string sOut;
		sOut.reserve(s.length() / 2);

		std::string extract;
		for (std::string::const_iterator pos = s.begin(); pos < s.end(); pos += 2)
		{
			extract.assign(pos, pos + 2);
			sOut.push_back(std::stoi(extract, nullptr, 16));
		}
		return sOut;
	}

	std::string xorstr(std::string value, std::string key)
	{
		std::string retval(value);
		long unsigned int klen = key.length();
		long unsigned int vlen = value.length();
		unsigned long int k = 0;
		unsigned long int v = 0;
		for (; v < vlen; v++) {
			retval[v] = value[v] ^ key[k];
			k = (++k < klen ? k : 0);
		}
		return retval;
	}
}