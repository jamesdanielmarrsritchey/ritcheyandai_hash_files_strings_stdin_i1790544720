#pragma once // Prevents this header from being included more than once in a translation unit.
#include <string> // Provides std::string for paths, checksum bytes/text, source labels, and delimiters.
// Description: Writes a checksum-file record to a requested file while enforcing overwrite protection and the requested field delimiter.
bool fun_958a9821_write_checksum_i1790544730_v3(const std::string&, const std::string&, const std::string&, const std::string&, bool, bool); // Declares the checksum writer.
