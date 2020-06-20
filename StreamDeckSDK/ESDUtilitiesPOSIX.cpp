//==============================================================================
/**
@file       ESDUtilities.cpp

@brief      Various filesystem and other utility functions

@copyright  (c) 2018, Corsair Memory, Inc.
      This source code is licensed under the MIT-style license found in the
LICENSE file.

**/
//==============================================================================

#include <fmt/format.h>
#include <unistd.h>

#include "ESDUtilities.h"
#include "ESDLogger.h"

void ESDUtilities::DoSleep(int inMilliseconds) {
  usleep(1000 * inMilliseconds);
}

std::string ESDUtilities::AddPathComponent(
  const std::string& inPath,
  const std::string& inComponentToAdd) {
  if (inPath.size() <= 0)
    return inComponentToAdd;

  char lastChar = inPath[inPath.size() - 1];

  bool pathEndsWithDelimiter = lastChar == '/';
  bool compStartsWithDelimiter = inComponentToAdd[0] == '/';

  std::string result;
  if (pathEndsWithDelimiter && compStartsWithDelimiter)
    result = inPath + inComponentToAdd.substr(1);
  else if (pathEndsWithDelimiter || compStartsWithDelimiter)
    result = inPath + inComponentToAdd;
  else
    result = inPath + '/' + inComponentToAdd;

  return result;
}

std::string ESDUtilities::GetParentDirectoryPath(const std::string& inPath) {
  if (inPath.empty()) {
    return "/";
  }
  if (inPath[0] != '/') {
    ESDDebug("Relative paths are not supported");
    return {};
  }

  auto idx = inPath.find_last_not_of('/');
  if (idx == std::string::npos) {
    return "/";
  }
  const auto trimmed(inPath.substr(0, idx + 1));

  idx = trimmed.find_last_of('/');
  if (idx == 0) {
    return "/";
  }
  return trimmed.substr(0, idx);
}

std::string ESDUtilities::GetFileName(const std::string& inPath) {
  //
  // Use the platform specific delimiter
  //
  auto delimiter = '/';

  //
  // Remove the trailing delimiters
  //
  const auto lastNonDelim = inPath.find_last_not_of(delimiter);
  if (lastNonDelim == std::string::npos) {
    return std::string();
  }

  const auto normalized = inPath.substr(0, lastNonDelim + 1);

  const auto delimIdx = inPath.find_last_of(delimiter);
  if (delimIdx == std::string::npos) {
    return normalized;
  }

  return normalized.substr(delimIdx + 1);
}

std::string ESDUtilities::GetPluginDirectoryPath() {
  static std::string dir;
  if (dir.empty()) {
    const std::string executable(GetPluginExecutablePath());
    const auto search = fmt::format(".sdPlugin/");
    const auto idx = executable.rfind(search);
    dir = executable.substr(0, idx + search.size() - 1);
  }
  return dir;
}