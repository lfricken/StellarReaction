# pragma once

/// Useful debugging item.
#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"

#ifdef _DEBUG
#define dout cout
#else
#define dout 0 && cout
#endif

