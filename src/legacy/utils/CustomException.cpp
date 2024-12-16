#include "CustomException.h"

CustomException::CustomException(char const *customMessage) {
  this->message = customMessage;
};

const char *CustomException::what() const throw() {
  return this->message;
};