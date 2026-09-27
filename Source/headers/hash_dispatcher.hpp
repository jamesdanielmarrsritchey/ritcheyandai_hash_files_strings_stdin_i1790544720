#pragma once // Prevents this header from being included more than once in a translation unit.
#include <istream> // Provides std::istream for the selected input source.
#include <string> // Provides std::string for algorithm names and checksum output.
// Description: Normalizes an algorithm name and sends a stream to the matching built-in implementation.
bool fun_f9ae9163_dispatch_hash_i1790544729_v1(std::istream&, const std::string&, std::string&); // Declares the hash dispatcher.
