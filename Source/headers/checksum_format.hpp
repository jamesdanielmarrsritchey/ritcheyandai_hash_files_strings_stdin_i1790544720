#pragma once // Prevents this header from being included more than once in a translation unit.
#include <string> // Provides std::string for hexadecimal input, format names, and formatted checksum output.
// Description: Identifies the canonical checksum output representation after a format argument has been normalized.
enum class ChecksumOutputFormat { // Defines supported output formats so other files can distinguish raw bytes from textual representations.
    Hexadecimal, // Represents lowercase hexadecimal digest text.
    Base64, // Represents RFC 4648 Base64 digest text.
    Binary, // Represents the raw digest bytes with no textual encoding.
    BinaryRepresentation, // Represents each digest byte as eight textual zero/one digits.
    DecimalRepresentation // Represents digest bytes as comma-separated decimal values from 0 through 255.
}; // Ends the checksum output format enumeration.
// Description: Converts a hexadecimal checksum to the requested supported representation and reports the canonical format selected.
bool fun_d2b6c4a1_format_checksum_i1790544732_v2(const std::string&, const std::string&, std::string&, ChecksumOutputFormat&); // Declares the checksum formatting function.
