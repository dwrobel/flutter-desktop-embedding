// Copyright 2018 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "utils.h"

#include <string.h>
#include <unistd.h>

#include <sstream>

namespace flutter {

static std::string GetExecutablePath() {
  char executable_path[1024] = {0};
  std::stringstream stream;
  stream << "/proc/" << getpid() << "/exe";
  auto path = stream.str();
  auto executable_path_size =
      ::readlink(path.c_str(), executable_path, sizeof(executable_path));
  if (executable_path_size <= 0) {
    return "";
  }
  return std::string{executable_path,
                     static_cast<size_t>(executable_path_size)};
}

std::string GetExecutableName() {
  auto path_string = GetExecutablePath();
  auto found = path_string.find_last_of('/');
  if (found == std::string::npos) {
    return "";
  }
  return path_string.substr(found + 1);
}

std::string GetExecutableDirectory() {
  auto path_string = GetExecutablePath();
  auto found = path_string.find_last_of('/');
  if (found == std::string::npos) {
    return "";
  }
  return path_string.substr(0, found + 1);
}

bool FileExistsAtPath(const std::string &path) {
  return ::access(path.c_str(), R_OK) == 0;
}

bool FlutterAssetBundleIsValid(const std::string &bundle_path) {
  if (!FileExistsAtPath(bundle_path)) {
    FLWAY_ERROR << "Bundle directory: '" << bundle_path << "' does not exist."
                << std::endl;
    return false;
  }

  auto kernel_path = bundle_path + std::string{"/kernel_blob.bin"};
  auto aotelf_path = bundle_path + std::string{"/"} + FlutterGetAppAotElfName();
  auto kernel = FileExistsAtPath(kernel_path);
  auto aotelf = FileExistsAtPath(aotelf_path);

  if (!(kernel || aotelf)) {
    FLWAY_ERROR << "Could not found neither " << kernel_path << " nor "
                << aotelf_path << std::endl;
    return false;
  }

  return true;
}

std::string FlutterGetAppAotElfName() {
  return std::string(
      "app-aot-elf.so");  // dw: TODO: There seems to be no convention name we
                          // could use, so let's temporary hardcode the file
                          // name.
}

}  // namespace flutter
