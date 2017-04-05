#include "utils.h"

std::string str_replace(std::string needle, std::string replace, std::string haystack) {
	while(haystack.find(needle) != std::string::npos) {
  		haystack.replace(haystack.find(needle), needle.size(), replace);
	}

	return haystack;
}