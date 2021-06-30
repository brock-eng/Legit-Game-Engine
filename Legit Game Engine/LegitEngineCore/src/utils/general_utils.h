#pragma once

#define ASSERT(x, message) if(!x) {std::cout << "[Legit Engine]: Assertion '" << #x << "' failed.  Reason: " << message; abort();} 

#define LOG(x) std::cout << "[Legit Engine]: " << x << std::endl;

