// Copyright 2018 The Flutter Authors. All rights reserved.
// Copyright 2019 Damian Wrobel <dwrobel@ertelnet.rybnik.pl>
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <flutter_embedder.h>

#include "macros.h"

namespace flutter {

std::string GetExecutableName();

std::string GetExecutableDirectory();

bool FileExistsAtPath(const std::string &path);

bool FlutterAssetBundleIsValid(const std::string &bundle_path);

std::string FlutterGetAppAotElfName();

}  // namespace flutter
