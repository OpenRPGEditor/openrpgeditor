#include "Database/Globals.hpp"

#include <fstream>
#include <iostream>

#include <regex>
#include <string>
#include <vector>

static void ReplaceStr(std::string& str, const std::string& oldStr, const std::string& newStr) {
  std::string::size_type pos = 0u;
  while ((pos = str.find(oldStr, pos)) != std::string::npos) {
    str.replace(pos, oldStr.length(), newStr);
    pos += newStr.length();
  }
}
std::string DecodeEnumName(std::string_view str) {
  if (str.size() <= 0) {
    return "UNKOWN_ENUM";
  }
  std::string result = str.data();

  /* NOTE: If you add any new encodings make sure
   * to notate them in the comment at the beginning
   * of this header file */
  ReplaceStr(result, "po_", "(");
  ReplaceStr(result, "_pc", ")");
  ReplaceStr(result, "bo_", "[");
  ReplaceStr(result, "_bc", "]");
  ReplaceStr(result, "_pe_", ".");
  result = result.substr(0, result.find("_del_"));
  ReplaceStr(result, "_deg", "°");
  ReplaceStr(result, "_da_", "-=");
  ReplaceStr(result, "_set_", "=");
  ReplaceStr(result, "_pl_", "+=");
  ReplaceStr(result, "_mul_", "*=");
  ReplaceStr(result, "_mod_", "%=");
  ReplaceStr(result, "_div_", "/=");
  ReplaceStr(result, "_lt_", "<");
  ReplaceStr(result, "_gt_", ">");
  ReplaceStr(result, "_not_", "!");

  // Remove the leading underscore if it exists
  if (result.front() == '_')
    result.erase(0, 1);

  /* Add new encodings *before* this or it will clobber them */
  ReplaceStr(result, "_", " ");

  result.shrink_to_fit();
  return result;
}
