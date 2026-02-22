///
/// @file: FileException.hpp
/// @description: Exception for when a file operation (e.g. load, save) fails
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef FILE_EXCEPTION_HPP
#define FILE_EXCEPTION_HPP

#include <exception>
#include <string>

class FileException: public std::exception {
private:
  std::string m_message;
public:
    FileException(): m_message("FileException") {}
    FileException(const std::string& message): m_message(message) {}

    virtual const char* what() const noexcept override {
        return m_message.c_str();
    }
};

#endif //FILE_EXCEPTION_HPP
