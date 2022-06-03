#include "../inc/Channel.hpp"

Channel::Channel(void) {
}

Channel::Channel(const std::string& chan_name, const std::string& chan_topic) : 
					_name(chan_name), _topic(chan_topic) {
}