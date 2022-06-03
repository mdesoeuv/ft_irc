#include "../inc/utils.hpp"

#include <string>
#include <vector>

void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result)
{
	std::string message = str;
	size_t		pos = 0;

	while (message.size() != 0)
	{
		// split in sub messages sep by \r\n
		pos = message.find(sep);
		if (pos > message.size())
			break;
		result.push_back(message.substr(0, pos));
		message.erase(0, pos + sep.size());
	}

}