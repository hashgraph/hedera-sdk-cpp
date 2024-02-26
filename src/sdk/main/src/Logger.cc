/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
    mLogger ? mLogger->trace(message) : spdlog::trace(message);
  }
}

//-----
void Logger::debug(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    mLogger ? mLogger->debug(message) : spdlog::debug(message);
  }
}

//-----
void Logger::info(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    mLogger ? mLogger->info(message) : spdlog::info(message);
  }
}

//-----
void Logger::warn(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    mLogger ? mLogger->warn(message) : spdlog::warn(message);
  }
}

//-----
void Logger::error(std::string_view message) const
{
  if (mCurrentLevel != LoggingLevel::SILENT)
  {
    mLogger ? mLogger->error(message) : spdlog::error(message);
  }
}

//-----
Logger& Logger::setLogger(const std::shared_ptr<spdlog::logger>& logger)
{
  mLogger = logger;
  return *this;
}

//-----
Logger& Logger::setLevel(Logger::LoggingLevel level)
{
  mPreviousLevel = mCurrentLevel;
  mCurrentLevel = level;
  switch (mCurrentLevel)
  {
    case LoggingLevel::TRACE:
      mLogger ? mLogger->set_level(spdlog::level::trace) : spdlog::set_level(spdlog::level::trace);
      break;
    case LoggingLevel::DEBUG:
      mLogger ? mLogger->set_level(spdlog::level::debug) : spdlog::set_level(spdlog::level::debug);
      break;
    case LoggingLevel::INFO:
      mLogger ? mLogger->set_level(spdlog::level::info) : spdlog::set_level(spdlog::level::info);
      break;
    case LoggingLevel::WARN:
      mLogger ? mLogger->set_level(spdlog::level::warn) : spdlog::set_level(spdlog::level::warn);
      break;
    case LoggingLevel::ERROR:
      mLogger ? mLogger->set_level(spdlog::level::err) : spdlog::set_level(spdlog::level::err);
      break;
    case LoggingLevel::SILENT:
      mLogger ? mLogger->set_level(spdlog::level::off) : spdlog::set_level(spdlog::level::off);
      break;
  }
  return *this;
}

//-----
Logger& Logger::setSilent(bool silent)
{
  mCurrentLevel = silent ? LoggingLevel::SILENT : mPreviousLevel;
  return *this;
}

} // namespace Hedera
