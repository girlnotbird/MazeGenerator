#ifndef CUSTOMEXCEPTION_HPP
#define CUSTOMEXCEPTION_HPP

#include <stdexcept>

class CustomException final : public std::exception
{
private:
    char const* message;

public:
    explicit CustomException(char const* customMessage);
    [[nodiscard]] const char* what() const noexcept override;
};

#endif //CUSTOMEXCEPTION_HPP
