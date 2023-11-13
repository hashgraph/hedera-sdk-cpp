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
#include "Logger.h"

namespace Hedera
{
//-----
Logger::Logger(Hedera::Logger::LoggingLevel level)
  : mCurrentLevel(level)
  , mPreviousLevel(level)
{
}

//-----
void Logger::trace(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    LOG4CXX_TRACE(mLogger, message);
  }
}

//-----
void Logger::debug(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    LOG4CXX_DEBUG(mLogger, message);
  }
}

//-----
void Logger::info(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    LOG4CXX_INFO(mLogger, message);
  }
}

//-----
void Logger::warn(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    LOG4CXX_WARN(mLogger, message);
  }
}

//-----
void Logger::error(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    LOG4CXX_ERROR(mLogger, message);
  }
}

//-----
Logger& Logger::setLogger(const log4cxx::LoggerPtr& logger)
{
  mLogger = logger;
  return *this;
}

//-----
Logger& Logger::setLevel(Logger::LoggingLevel level)
{
  mPreviousLevel = mCurrentLevel;
  mCurrentLevel = level;
  return *this;
}

//-----
Logger& Logger::setSilent(bool silent)
{
  mCurrentLevel = silent ? LoggingLevel::SILENT : mPreviousLevel;
  return *this;
}

} // namespace Hedera
