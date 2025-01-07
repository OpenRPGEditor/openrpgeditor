#include "Database/Globals.hpp"

#include <fstream>
#include <iostream>

#include <regex>
#include <string>
#include <vector>

void ReplaceStr(std::string& str, const std::string& oldStr, const std::string& newStr) {
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
  ReplaceStr(result, "_plu_", "+");
  ReplaceStr(result, "_daa_", "-");
  ReplaceStr(result, "_deg", "°");
  ReplaceStr(result, "_da_", "-=");
  ReplaceStr(result, "_set_", "=");
  ReplaceStr(result, "_pl_", "+=");
  ReplaceStr(result, "_mul_", "*=");
  ReplaceStr(result, "_mod_", "%=");
  ReplaceStr(result, "_div_", "/=");
  ReplaceStr(result, "_lt_", "<");
  ReplaceStr(result, "_lteq_", "\u2264");
  ReplaceStr(result, "_gt_", ">");
  ReplaceStr(result, "_gteq_", "\u2265");
  ReplaceStr(result, "_not_", "!");

  // Remove the leading underscore if it exists
  if (result.front() == '_')
    result.erase(0, 1);

  /* Add new encodings *before* this or it will clobber them */
  ReplaceStr(result, "_", " ");

  result.shrink_to_fit();
  auto trStr = moloader::gettext(result.c_str());
  return trStr ? trStr : result;
}

std::string UndectorateEnumName(std::string_view str) {
  if (str.size() <= 0) {
    return "UNKOWN_ENUM";
  }
  std::string result = str.data();

  /* NOTE: If you add any new encodings make sure
   * to notate them in the comment at the beginning
   * of this header file */
  ReplaceStr(result, "po_", "");
  ReplaceStr(result, "_pc", "");
  ReplaceStr(result, "bo_", "");
  ReplaceStr(result, "_bc", "");
  ReplaceStr(result, "_pe_", "");
  ReplaceStr(result, "_del_", "");
  ReplaceStr(result, "_plu_", "");
  ReplaceStr(result, "_daa_", "");
  ReplaceStr(result, "_deg", "");
  ReplaceStr(result, "_da_", "");
  ReplaceStr(result, "_set_", "");
  ReplaceStr(result, "_pl_", "");
  ReplaceStr(result, "_mul_", "");
  ReplaceStr(result, "_mod_", "");
  ReplaceStr(result, "_div_", "");
  ReplaceStr(result, "_lteq_", "");
  ReplaceStr(result, "_lt_", "");
  ReplaceStr(result, "_gt_", "");
  ReplaceStr(result, "_gteq_", "");
  ReplaceStr(result, "_not_", "");

  // Remove the leading underscore if it exists
  if (result.front() == '_')
    result.erase(0, 1);

  /* Add new encodings *before* this or it will clobber them */
  ReplaceStr(result, "_", " ");

  result.shrink_to_fit();
  auto trStr = moloader::gettext(result.c_str());
  return trStr ? trStr : result;
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream ss(str);
  std::string token;

  int index = 0;
  while (std::getline(ss, token, delimiter)) {
    index++;
    tokens.push_back(token);
  }

  return tokens;
}

std::string& trim(std::string& s) {
  // Find first non whitespace char in StrToTrim
  std::string::size_type first = s.find_first_not_of(' ');

  // Check whether something went wrong?
  if (first == std::string::npos) {
    first = 0;
  }

  // Find last non whitespace char from StrToTrim
  std::string::size_type last = s.find_last_not_of(' ');

  // If something didn't go wrong, Last will be recomputed to get real length of substring
  if (last != std::string::npos) {
    last = (last + 1) - first;
  }

  // Copy such a string to TrimmedString
  s = s.substr(first, last);

  return s;
}