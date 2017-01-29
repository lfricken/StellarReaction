# pragma once

/// Useful debugging item.
#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"

#ifdef _DEBUG
#define dout std::cout
#else
#define dout 0 && std::cout
#endif

