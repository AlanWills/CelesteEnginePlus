#include "Logging/StdoutLogger.h"

#include <iostream>


namespace Celeste
{
  //------------------------------------------------------------------------------------------------
  void StdoutLogger::log(const std::string& message, Verbosity verbosity, const char* function, const char* file, int line)
  {
    switch (verbosity)
    {
    case Verbosity::kInfo:
        std::cout << "INFO: ";
        break;

    case Verbosity::kWarning:
      std::cout << "WARNING: ";
      break;

    case Verbosity::kError:
      std::cout << "ERROR: ";
      break;

    case Verbosity::kCriticalError:
      std::cout << "CRITICAL ERROR: ";
      break;

    case Verbosity::kRaw:
      std::cout << "RAW: ";
      break;
    }

    std::cout << message << " " << function << " " << file << " " << line << std::endl;
  }
}