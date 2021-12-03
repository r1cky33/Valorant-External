#pragma once

namespace crypt {
	std::string hex2bin(std::string const& s);
	std::string xorstr(std::string value, std::string key);
}