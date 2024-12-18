#include "../include/CustomException.hpp"

CustomException::CustomException(char const* customMessage)
{
    this->message = customMessage;
};

const char* CustomException::what() const noexcept
{
    return this->message;
};
