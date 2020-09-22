// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <flutter/flutter_window_controller.h>
#include <linux/limits.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "flutter/generated_plugin_registrant.h"
#include "utils.h"

namespace flutter {

static std::string GetICUDataPath() {
  auto base_directory = GetExecutableDirectory();
  if (base_directory == "") {
    base_directory = ".";
  }

  std::string data_directory = base_directory + "/data";
  std::string icu_data_path = data_directory + "/icudtl.dat";

  do {
    if (std::ifstream(icu_data_path)) {
      std::cout << "Using: " << icu_data_path << std::endl;
      break;
    }

    icu_data_path = "/usr/share/flutter/icudtl.dat";

    if (std::ifstream(icu_data_path)) {
      std::cout << "Using: " << icu_data_path << std::endl;
      break;
    }

    std::cerr << "Unnable to locate icudtl.dat file" << std::endl;
    icu_data_path = "";
  } while (0);

  return icu_data_path;
}

static void PrintUsage() {
  std::cerr << "Flutter Wayland Embedder" << std::endl << std::endl;
  std::cerr << "========================" << std::endl;
  std::cerr << "Usage: `" << GetExecutableName()
            << " <asset_bundle_path> <flutter_flags>`" << std::endl
            << std::endl;
  std::cerr << R"~(
  This utility runs an instance of a Flutter application and renders using
  Wayland core protocols.

  The Flutter tools can be obtained at https://flutter.io/

  asset_bundle_path: The Flutter application code needs to be snapshotted using
                     the Flutter tools and the assets packaged in the appropriate
                     location. This can be done for any Flutter application by
                     running `flutter build bundle` while in the directory of a
                     valid Flutter project. This should package all the code and
                     assets in the "build/flutter_assets" directory. Specify this
                     directory as the first argument to this utility.

      flutter_flags: Typically empty. These extra flags are passed directly to the
                     Flutter engine. To see all supported flags, run
                     `flutter_tester --help` using the test binary included in the
                     Flutter tools.
  )~" << std::endl;
}

}  // namespace flutter

int main(int argc, char **argv) {
  // Arguments for the Flutter Engine.
  std::vector<std::string> arguments;
  for (int i = 1; i < argc; ++i) {
    arguments.push_back(argv[i]);
  }

  if (arguments.size() == 0) {
    std::cerr << "   <Invalid Arguments>   " << std::endl;
    flutter::PrintUsage();
    return EXIT_FAILURE;
  }

  const auto asset_bundle_path = arguments[0];

  auto icu_data_path = flutter::GetICUDataPath();

  if (icu_data_path == "") {
    std::cerr << "Unnable to locate icudtl.dat file" << std::endl;
    flutter::PrintUsage();
    return EXIT_FAILURE;
  }

  flutter::FlutterWindowController flutter_controller(icu_data_path);
  flutter::WindowProperties window_properties = {};
  window_properties.title = "Flutter Desktop Example";
  window_properties.width = 1920;
  window_properties.height = 1080;

  // Start the engine.
  if (!flutter_controller.CreateWindow(window_properties, asset_bundle_path,
                                       arguments)) {
    return EXIT_FAILURE;
  }
  RegisterPlugins(&flutter_controller);

  // Run until the window is closed.
  while (flutter_controller.RunEventLoopWithTimeout(
      std::chrono::milliseconds::max())) {
  }
  return EXIT_SUCCESS;
}
