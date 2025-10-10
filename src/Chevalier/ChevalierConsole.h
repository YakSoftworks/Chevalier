#pragma once

#include <stdexcept>

// Standard Message

#define CHEV_MESSAGE_LOG(message) std::cout << message << std::endl;

// Message as Error

#define CHEV_MESSAGE_ERROR(message) throw std::runtime_error(message);


