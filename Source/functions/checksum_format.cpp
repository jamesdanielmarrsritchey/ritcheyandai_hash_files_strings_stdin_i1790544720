// Description: Converts hexadecimal digest text produced by the built-in hash implementations into hexadecimal, Base64, raw binary, textual binary, or decimal checksum output.
#include "checksum_format.hpp" // Imports the checksum-formatting function declaration and canonical format enumeration.
#include <cctype> // Provides std::tolower for case-insensitive format names.
#include <cstddef> // Provides std::size_t for string and byte indexes.
#include <string> // Provides std::string and std::to_string for format normalization and output assembly.
#include <vector> // Provides byte storage while converting hexadecimal digest text.
bool fun_d2b6c4a1_format_checksum_i1790544732_v2(const std::string& var_d2b6c4a1_hex_checksum_str, const std::string& var_d2b6c4a1_requested_format_str, std::string& var_d2b6c4a1_formatted_checksum_str, ChecksumOutputFormat& var_d2b6c4a1_output_format_uns) { // Starts checksum representation conversion.
    std::string var_d2b6c4a1_format_str = var_d2b6c4a1_requested_format_str; // Copies the requested format so it can be normalized without modifying caller data.
    for (std::size_t var_d2b6c4a1_index_num = 0U; var_d2b6c4a1_index_num < var_d2b6c4a1_format_str.size(); ++var_d2b6c4a1_index_num) { // Visits every format-name character.
        unsigned char var_d2b6c4a1_character_num = static_cast<unsigned char>(var_d2b6c4a1_format_str[var_d2b6c4a1_index_num]); // Converts the byte to unsigned char before using the character classification API.
        var_d2b6c4a1_format_str[var_d2b6c4a1_index_num] = static_cast<char>(std::tolower(var_d2b6c4a1_character_num)); // Normalizes the current format-name character to lowercase.
    } // Ends format-name lowercasing.
    if ((var_d2b6c4a1_format_str == "hex") || (var_d2b6c4a1_format_str == "hexidecimal")) { // Checks common shorthand and the historically used alternate spelling for hexadecimal.
        var_d2b6c4a1_format_str = "hexadecimal"; // Converts the accepted alias to the canonical hexadecimal format name.
    } // Ends hexadecimal alias normalization.
    if ((var_d2b6c4a1_format_str == "binary-representation") || (var_d2b6c4a1_format_str == "binary_representation") || (var_d2b6c4a1_format_str == "binaryrepresentation")) { // Checks convenient aliases for the textual bit representation.
        var_d2b6c4a1_format_str = "binary representation"; // Converts the accepted alias to the canonical binary-representation name.
    } // Ends binary-representation alias normalization.
    if ((var_d2b6c4a1_format_str == "decimal-representation") || (var_d2b6c4a1_format_str == "decimal_representation") || (var_d2b6c4a1_format_str == "decimalrepresentation")) { // Checks convenient aliases for the decimal byte representation.
        var_d2b6c4a1_format_str = "decimal representation"; // Converts the accepted alias to the canonical decimal-representation name.
    } // Ends decimal-representation alias normalization.
    if (var_d2b6c4a1_format_str == "hexadecimal") { // Checks whether the caller requested the default hexadecimal representation.
        var_d2b6c4a1_output_format_uns = ChecksumOutputFormat::Hexadecimal; // Reports the canonical hexadecimal format to the caller.
        var_d2b6c4a1_formatted_checksum_str = var_d2b6c4a1_hex_checksum_str; // Returns the built-in lowercase hexadecimal digest without transformation.
        return true; // Reports successful hexadecimal formatting.
    } // Ends hexadecimal output handling.
    if ((var_d2b6c4a1_format_str != "base64") && (var_d2b6c4a1_format_str != "binary") && (var_d2b6c4a1_format_str != "binary representation") && (var_d2b6c4a1_format_str != "decimal representation")) { // Checks whether the normalized format is one of the remaining supported representations.
        return false; // Reports failure for unsupported format names.
    } // Ends supported-format validation.
    if ((var_d2b6c4a1_hex_checksum_str.size() % 2U) != 0U) { // Checks that the hexadecimal digest contains complete byte pairs.
        return false; // Reports failure when the internal hexadecimal digest length cannot represent whole bytes.
    } // Ends hexadecimal length validation.
    std::vector<unsigned char> var_d2b6c4a1_bytes_arr; // Creates storage for checksum bytes decoded from hexadecimal text.
    var_d2b6c4a1_bytes_arr.reserve(var_d2b6c4a1_hex_checksum_str.size() / 2U); // Reserves the exact number of bytes represented by the hexadecimal input.
    for (std::size_t var_d2b6c4a1_index_num = 0U; var_d2b6c4a1_index_num < var_d2b6c4a1_hex_checksum_str.size(); var_d2b6c4a1_index_num += 2U) { // Processes each hexadecimal byte pair.
        char var_d2b6c4a1_high_character_uns = var_d2b6c4a1_hex_checksum_str[var_d2b6c4a1_index_num]; // Reads the high hexadecimal digit of the current byte.
        char var_d2b6c4a1_low_character_uns = var_d2b6c4a1_hex_checksum_str[var_d2b6c4a1_index_num + 1U]; // Reads the low hexadecimal digit of the current byte.
        int var_d2b6c4a1_high_value_num = -1; // Initializes the decoded high nibble to an invalid sentinel value.
        int var_d2b6c4a1_low_value_num = -1; // Initializes the decoded low nibble to an invalid sentinel value.
        if ((var_d2b6c4a1_high_character_uns >= '0') && (var_d2b6c4a1_high_character_uns <= '9')) { // Checks whether the high digit is decimal hexadecimal syntax.
            var_d2b6c4a1_high_value_num = static_cast<int>(var_d2b6c4a1_high_character_uns - '0'); // Converts the decimal high digit to its numeric nibble value.
        } else if ((var_d2b6c4a1_high_character_uns >= 'a') && (var_d2b6c4a1_high_character_uns <= 'f')) { // Checks whether the high digit is lowercase alphabetic hexadecimal syntax.
            var_d2b6c4a1_high_value_num = static_cast<int>(var_d2b6c4a1_high_character_uns - 'a') + 10; // Converts the lowercase high digit to its numeric nibble value.
        } else if ((var_d2b6c4a1_high_character_uns >= 'A') && (var_d2b6c4a1_high_character_uns <= 'F')) { // Checks whether the high digit is uppercase alphabetic hexadecimal syntax.
            var_d2b6c4a1_high_value_num = static_cast<int>(var_d2b6c4a1_high_character_uns - 'A') + 10; // Converts the uppercase high digit to its numeric nibble value.
        } // Ends high-nibble decoding.
        if ((var_d2b6c4a1_low_character_uns >= '0') && (var_d2b6c4a1_low_character_uns <= '9')) { // Checks whether the low digit is decimal hexadecimal syntax.
            var_d2b6c4a1_low_value_num = static_cast<int>(var_d2b6c4a1_low_character_uns - '0'); // Converts the decimal low digit to its numeric nibble value.
        } else if ((var_d2b6c4a1_low_character_uns >= 'a') && (var_d2b6c4a1_low_character_uns <= 'f')) { // Checks whether the low digit is lowercase alphabetic hexadecimal syntax.
            var_d2b6c4a1_low_value_num = static_cast<int>(var_d2b6c4a1_low_character_uns - 'a') + 10; // Converts the lowercase low digit to its numeric nibble value.
        } else if ((var_d2b6c4a1_low_character_uns >= 'A') && (var_d2b6c4a1_low_character_uns <= 'F')) { // Checks whether the low digit is uppercase alphabetic hexadecimal syntax.
            var_d2b6c4a1_low_value_num = static_cast<int>(var_d2b6c4a1_low_character_uns - 'A') + 10; // Converts the uppercase low digit to its numeric nibble value.
        } // Ends low-nibble decoding.
        if ((var_d2b6c4a1_high_value_num < 0) || (var_d2b6c4a1_low_value_num < 0)) { // Checks whether either hexadecimal digit failed validation.
            return false; // Reports failure because the internal digest contains a non-hexadecimal character.
        } // Ends hexadecimal character validation.
        unsigned char var_d2b6c4a1_byte_num = static_cast<unsigned char>((var_d2b6c4a1_high_value_num << 4) | var_d2b6c4a1_low_value_num); // Combines the two validated nibbles into one checksum byte.
        var_d2b6c4a1_bytes_arr.push_back(var_d2b6c4a1_byte_num); // Appends the decoded checksum byte in its original digest order.
    } // Ends hexadecimal-to-byte conversion.
    if (var_d2b6c4a1_format_str == "binary") { // Checks whether the caller requested the raw digest bytes.
        var_d2b6c4a1_output_format_uns = ChecksumOutputFormat::Binary; // Reports raw-binary output to the caller so standard output can avoid adding textual bytes such as a newline.
        var_d2b6c4a1_formatted_checksum_str.assign(reinterpret_cast<const char*>(var_d2b6c4a1_bytes_arr.data()), var_d2b6c4a1_bytes_arr.size()); // Copies the exact digest bytes, including possible NUL bytes, into the binary-safe string container.
        return true; // Reports successful raw-binary formatting.
    } // Ends raw binary output handling.
    if (var_d2b6c4a1_format_str == "binary representation") { // Checks whether the caller requested textual binary digits.
        var_d2b6c4a1_output_format_uns = ChecksumOutputFormat::BinaryRepresentation; // Reports the textual bit representation to the caller.
        var_d2b6c4a1_formatted_checksum_str.clear(); // Clears caller-provided output before assembling the binary representation.
        var_d2b6c4a1_formatted_checksum_str.reserve(var_d2b6c4a1_bytes_arr.size() * 8U); // Reserves exactly eight output characters for each checksum byte.
        for (std::size_t var_d2b6c4a1_byte_index_num = 0U; var_d2b6c4a1_byte_index_num < var_d2b6c4a1_bytes_arr.size(); ++var_d2b6c4a1_byte_index_num) { // Visits every checksum byte in digest order.
            for (int var_d2b6c4a1_bit_index_num = 7; var_d2b6c4a1_bit_index_num >= 0; --var_d2b6c4a1_bit_index_num) { // Visits each bit from most significant to least significant.
                bool var_d2b6c4a1_bit_set_boo = ((var_d2b6c4a1_bytes_arr[var_d2b6c4a1_byte_index_num] >> var_d2b6c4a1_bit_index_num) & 0x01U) == 0x01U; // Tests the current checksum bit explicitly.
                var_d2b6c4a1_formatted_checksum_str.push_back(var_d2b6c4a1_bit_set_boo == true ? '1' : '0'); // Appends one textual binary digit for the current bit.
            } // Ends current-byte bit conversion.
        } // Ends textual binary conversion.
        return true; // Reports successful textual binary formatting.
    } // Ends binary-representation output handling.
    if (var_d2b6c4a1_format_str == "decimal representation") { // Checks whether the caller requested comma-separated decimal byte values.
        var_d2b6c4a1_output_format_uns = ChecksumOutputFormat::DecimalRepresentation; // Reports the textual decimal representation to the caller.
        var_d2b6c4a1_formatted_checksum_str.clear(); // Clears caller-provided output before assembling decimal values.
        for (std::size_t var_d2b6c4a1_byte_index_num = 0U; var_d2b6c4a1_byte_index_num < var_d2b6c4a1_bytes_arr.size(); ++var_d2b6c4a1_byte_index_num) { // Visits every checksum byte in digest order.
            if (var_d2b6c4a1_byte_index_num > 0U) { // Checks whether this is not the first decimal byte value.
                var_d2b6c4a1_formatted_checksum_str.push_back(','); // Separates consecutive decimal byte values with one comma and no additional whitespace.
            } // Ends decimal-value separator handling.
            var_d2b6c4a1_formatted_checksum_str += std::to_string(static_cast<unsigned int>(var_d2b6c4a1_bytes_arr[var_d2b6c4a1_byte_index_num])); // Appends the current byte as an unsigned decimal integer from 0 through 255.
        } // Ends decimal representation conversion.
        return true; // Reports successful decimal representation formatting.
    } // Ends decimal-representation output handling.
    var_d2b6c4a1_output_format_uns = ChecksumOutputFormat::Base64; // Reports Base64 as the only remaining supported normalized output format.
    const std::string var_d2b6c4a1_base64_alphabet_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // Defines the standard RFC 4648 Base64 alphabet without relying on an encoding library.
    var_d2b6c4a1_formatted_checksum_str.clear(); // Clears caller-provided output before assembling Base64 text.
    var_d2b6c4a1_formatted_checksum_str.reserve(((var_d2b6c4a1_bytes_arr.size() + 2U) / 3U) * 4U); // Reserves the standard padded Base64 output length.
    for (std::size_t var_d2b6c4a1_index_num = 0U; var_d2b6c4a1_index_num < var_d2b6c4a1_bytes_arr.size(); var_d2b6c4a1_index_num += 3U) { // Processes checksum bytes in Base64 groups of up to three.
        std::size_t var_d2b6c4a1_remaining_num = var_d2b6c4a1_bytes_arr.size() - var_d2b6c4a1_index_num; // Calculates how many checksum bytes remain in the current Base64 group.
        unsigned int var_d2b6c4a1_first_num = static_cast<unsigned int>(var_d2b6c4a1_bytes_arr[var_d2b6c4a1_index_num]); // Loads the first byte of the current Base64 group.
        unsigned int var_d2b6c4a1_second_num = var_d2b6c4a1_remaining_num > 1U ? static_cast<unsigned int>(var_d2b6c4a1_bytes_arr[var_d2b6c4a1_index_num + 1U]) : 0U; // Loads the second byte when present or zero-fills it for padding calculations.
        unsigned int var_d2b6c4a1_third_num = var_d2b6c4a1_remaining_num > 2U ? static_cast<unsigned int>(var_d2b6c4a1_bytes_arr[var_d2b6c4a1_index_num + 2U]) : 0U; // Loads the third byte when present or zero-fills it for padding calculations.
        unsigned int var_d2b6c4a1_combined_num = (var_d2b6c4a1_first_num << 16U) | (var_d2b6c4a1_second_num << 8U) | var_d2b6c4a1_third_num; // Combines up to three checksum bytes into the 24-bit Base64 working value.
        var_d2b6c4a1_formatted_checksum_str.push_back(var_d2b6c4a1_base64_alphabet_str[(var_d2b6c4a1_combined_num >> 18U) & 0x3FU]); // Appends the first six-bit Base64 character.
        var_d2b6c4a1_formatted_checksum_str.push_back(var_d2b6c4a1_base64_alphabet_str[(var_d2b6c4a1_combined_num >> 12U) & 0x3FU]); // Appends the second six-bit Base64 character.
        var_d2b6c4a1_formatted_checksum_str.push_back(var_d2b6c4a1_remaining_num > 1U ? var_d2b6c4a1_base64_alphabet_str[(var_d2b6c4a1_combined_num >> 6U) & 0x3FU] : '='); // Appends the third Base64 character or standard padding when only one byte remains.
        var_d2b6c4a1_formatted_checksum_str.push_back(var_d2b6c4a1_remaining_num > 2U ? var_d2b6c4a1_base64_alphabet_str[var_d2b6c4a1_combined_num & 0x3FU] : '='); // Appends the fourth Base64 character or standard padding when fewer than three bytes remain.
    } // Ends Base64 conversion.
    return true; // Reports successful Base64 formatting.
} // Ends checksum representation conversion.
