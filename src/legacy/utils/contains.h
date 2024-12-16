#ifndef _CONTAINS_H
#define _CONTAINS_H

template<class C, typename T>
bool contains(C&& c, T e) { return std::find(std::begin(c), std::end(c), e) != std::end(c); };

#endif // _CONTAINS_H