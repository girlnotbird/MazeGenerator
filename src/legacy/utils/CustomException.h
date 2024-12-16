#ifndef _CUSTOM_EXCEPTION_H
#define _CUSTOM_EXCEPTION_H

#include <exception>
#include <string>

class CustomException : public std::exception
{

private:
  char const *message;

public:
  CustomException(char const *customMessage);
  virtual const char *what() const throw();
};

#endif // _CUSTOM_EXCEPTION_H