#pragma once

#include "Database/Trait.hpp"
#include <vector>

struct DatabaseEditor;

void TraitsEditor(std::vector<Trait>& traits, DatabaseEditor* dbEditor);