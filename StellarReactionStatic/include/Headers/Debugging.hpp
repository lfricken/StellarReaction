# pragma once

/// Useful debugging item.
#define FILELINE "\nfile:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"
#define WARNING std::cout << FILELINE

#ifdef _DEBUG
#define dout std::cout
#else
#define dout 0 && std::cout
#endif

