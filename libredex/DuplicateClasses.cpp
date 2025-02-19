/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DuplicateClasses.h"

#include "Trace.h"
#include <boost/algorithm/string/predicate.hpp>
#include <vector>

namespace dup_classes {

// Global allowlist for duplicated classes
std::vector<std::string> g_dup_class_allowlist{
    "Lcom/facebook/soloader/MergedSoMapping;", "Ljunit/framework/TestSuite;",
    // Java 8 desugaring classes generated by D8. See this link for more info:
    // https://r8.googlesource.com/r8/+/refs/heads/d8-1.5/src/main/java/com/
    //   android/tools/r8/utils/ProgramClassCollection.java#72
    "L$r8$java8methods$utility", "Ljava/util/stream/Stream$-",
    "Ljava/util/stream/IntStream$-", "Landroid/os/IBinder$-"};

// Read allowed duplicate class list from config.
void read_dup_class_allowlist(const JsonWrapper& json_cfg) {
  std::vector<std::string> dups;
  json_cfg.get("dup_class_allowlist", {}, dups);
  for (const auto& name : dups) {
    g_dup_class_allowlist.emplace_back(name);
  }

  TRACE(MAIN, 1, "dup_class_allowlist: { ");
  for (const auto& allowlist_name : g_dup_class_allowlist) {
    TRACE(MAIN, 1, "  %s", allowlist_name.c_str());
  }
  TRACE(MAIN, 1, "}");
}

// Return true if the cls is among one of the known allowed duplicated
// classes.
bool is_known_dup(DexClass* cls) {
  return std::find_if(g_dup_class_allowlist.begin(),
                      g_dup_class_allowlist.end(),
                      [cls](const std::string& name) {
                        return boost::starts_with(cls->get_name()->str(), name);
                      }) != g_dup_class_allowlist.end();
}

} // namespace dup_classes
