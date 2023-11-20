/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef HEDERA_SDK_CPP_LOGGER_H_
#define HEDERA_SDK_CPP_LOGGER_H_

#include "Defaults.h"

#include <log4cxx/logger.h>
#include <memory>
#include <string_view>

namespace Hedera
{
/**
 * Logger class used by the Hedera C++ SDK. In essence, it's a wrapper for a log4cxx logger instance and provides easier
 * usage.
 */
class Logger
{
public:
  /**
   * Enumeration of logging levels used by Loggers.
   */
  enum class LoggingLevel
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    SILENT
  };

  Logger() = default;

  /**
   * Construct with a specific logging level.
   *
   * @param level The level at which this Logger should log.
   */
  explicit Logger(LoggingLevel level);

  /**
   * Log a trace-level message.
   *
   * @param message The trace-level message to log.
   */
  void trace(std::string_view message) const;

  /**
   * Log a debug-level message.
   *
   * @param message The debug-level message to log.
   */
  void debug(std::string_view message) const;

  /**
   * Log a info-level message.
   *
   * @param message The info-level message to log.
   */
  void info(std::string_view message) const;

  /**
   * Log a warn-level message.
   *
   * @param message The warn-level message to log.
   */
  void warn(std::string_view message) const;

  /**
   * Log a error-level message.
   *
   * @param message The error-level message to log.
   */
  void error(std::string_view message) const;

  /**
   * Set the log4cxx logger this Logger should use.
   *
   * @param logger The log4cxx logger to use.
   * @return A reference to this Logger with the newly-set log4cxx logger.
   */
  Logger& setLogger(const log4cxx::LoggerPtr& logger);

  /**
   * Set the log level for this Logger.
   *
   * @param level The level at which this Logger should log.
   * @return A reference to this Logger with the newly-set logging level.
   */
  Logger& setLevel(LoggingLevel level);

  /**
   * Silence this Logger from logging.
   *
   * @param silent \c TRUE if this Logger should be silent, otherwise \c FALSE.
   * @param A reference to this Logger with the newly-set silence policy.
   */
  Logger& setSilent(bool silent);

  /**
   * Get the log4cxx logger used by this Logger.
   *
   * @return The log4cxx logger used by this Logger.
   */
  [[nodiscard]] inline log4cxx::LoggerPtr getLogger() const { return mLogger; }

  /**
   * Get the LoggingLevel currently being used by this Logger.
   *
   * @return The LoggingLevel currently being used by this Logger.
   */
  [[nodiscard]] inline LoggingLevel getLevel() const { return mCurrentLevel; }

private:
  /**
   * The wrapped log4cxx logger.
   */
  log4cxx::LoggerPtr mLogger = log4cxx::Logger::getLogger(DEFAULT_LOGGER_NAME);

  /**
   * The current logger level.
   */
  LoggingLevel mCurrentLevel = LoggingLevel::TRACE;

  /**
   * The previous logger level.
   */
  LoggingLevel mPreviousLevel = LoggingLevel::TRACE;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_LOGGER_H_
