/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "DexClass.h"
#include "Pass.h"

class IntraDexInlinePass : public Pass {
 public:
  IntraDexInlinePass() : Pass("IntraDexInlinePass") {}

  void run_pass(DexStoresVector&, ConfigFiles&, PassManager&) override;
};
