// Description: Normalizes algorithm names and dispatches one input stream to the matching built-in hashing implementation.
#include "hash_dispatcher.hpp" // Imports the dispatcher declaration.
#include "hash_crc32.hpp" // Imports the built-in CRC32 implementation.
#include "hash_md5.hpp" // Imports the built-in MD5 implementation.
#include "hash_sha1.hpp" // Imports the built-in SHA-1 implementation.
#include "hash_sha256.hpp" // Imports the built-in SHA-256 implementation.
#include "hash_sha512.hpp" // Imports the built-in SHA-512 implementation.
#include "hash_sha3_256.hpp" // Imports the built-in SHA3-256 implementation.
#include "hash_sha3_512.hpp" // Imports the built-in SHA3-512 implementation.
#include <cctype> // Provides std::tolower for case-insensitive algorithm names.
#include <string> // Provides std::string for normalized algorithm names and output.
bool fun_f9ae9163_dispatch_hash_i1790544729_v1(std::istream& var_f9ae9163_input_stream_obj, const std::string& var_f9ae9163_algorithm_str, std::string& var_f9ae9163_checksum_str) { // Starts algorithm normalization and dispatch.
    std::string var_f9ae9163_normalized_algorithm_str = var_f9ae9163_algorithm_str; // Copies the requested name so it can be normalized without modifying caller data.
    for (std::size_t var_f9ae9163_index_num = 0U; var_f9ae9163_index_num < var_f9ae9163_normalized_algorithm_str.size(); ++var_f9ae9163_index_num) { // Visits every character in the algorithm name.
        unsigned char var_f9ae9163_character_num = static_cast<unsigned char>(var_f9ae9163_normalized_algorithm_str[var_f9ae9163_index_num]); // Converts the current byte to unsigned char before passing it to the character classification API.
        var_f9ae9163_normalized_algorithm_str[var_f9ae9163_index_num] = static_cast<char>(std::tolower(var_f9ae9163_character_num)); // Converts the current algorithm-name character to lowercase.
    } // Ends algorithm-name lowercasing.
    if (var_f9ae9163_normalized_algorithm_str == "sha-1") { // Checks for the common hyphenated SHA-1 alias.
        var_f9ae9163_normalized_algorithm_str = "sha1"; // Converts the alias to the canonical SHA-1 name.
    } else if (var_f9ae9163_normalized_algorithm_str == "sha-256") { // Checks for the common hyphenated SHA-256 alias.
        var_f9ae9163_normalized_algorithm_str = "sha256"; // Converts the alias to the canonical SHA-256 name.
    } else if (var_f9ae9163_normalized_algorithm_str == "sha-512") { // Checks for the common hyphenated SHA-512 alias.
        var_f9ae9163_normalized_algorithm_str = "sha512"; // Converts the alias to the canonical SHA-512 name.
    } else if ((var_f9ae9163_normalized_algorithm_str == "sha3256") || (var_f9ae9163_normalized_algorithm_str == "sha3_256")) { // Checks for compact or underscore SHA3-256 aliases.
        var_f9ae9163_normalized_algorithm_str = "sha3-256"; // Converts the alias to the canonical SHA3-256 name.
    } else if ((var_f9ae9163_normalized_algorithm_str == "sha3512") || (var_f9ae9163_normalized_algorithm_str == "sha3_512")) { // Checks for compact or underscore SHA3-512 aliases.
        var_f9ae9163_normalized_algorithm_str = "sha3-512"; // Converts the alias to the canonical SHA3-512 name.
    } // Ends algorithm alias normalization.
    if (var_f9ae9163_normalized_algorithm_str == "crc32") { // Checks whether CRC32 was requested.
        return fun_c55b1805_hash_crc32_stream_i1790544722_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in CRC32 implementation and returns its success state.
    } // Ends CRC32 dispatch.
    if (var_f9ae9163_normalized_algorithm_str == "md5") { // Checks whether MD5 was requested.
        return fun_fc244f77_hash_md5_stream_i1790544723_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in MD5 implementation and returns its success state.
    } // Ends MD5 dispatch.
    if (var_f9ae9163_normalized_algorithm_str == "sha1") { // Checks whether SHA-1 was requested.
        return fun_99bdf04b_hash_sha1_stream_i1790544724_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in SHA-1 implementation and returns its success state.
    } // Ends SHA-1 dispatch.
    if (var_f9ae9163_normalized_algorithm_str == "sha256") { // Checks whether SHA-256 was requested.
        return fun_b8a95e6b_hash_sha256_stream_i1790544725_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in SHA-256 implementation and returns its success state.
    } // Ends SHA-256 dispatch.
    if (var_f9ae9163_normalized_algorithm_str == "sha512") { // Checks whether SHA-512 was requested.
        return fun_a96b797b_hash_sha512_stream_i1790544726_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in SHA-512 implementation and returns its success state.
    } // Ends SHA-512 dispatch.
    if (var_f9ae9163_normalized_algorithm_str == "sha3-256") { // Checks whether SHA3-256 was requested.
        return fun_abe7a20a_hash_sha3_256_stream_i1790544727_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in SHA3-256 implementation and returns its success state.
    } // Ends SHA3-256 dispatch.
    if (var_f9ae9163_normalized_algorithm_str == "sha3-512") { // Checks whether SHA3-512 was requested.
        return fun_e09a33f2_hash_sha3_512_stream_i1790544728_v1(var_f9ae9163_input_stream_obj, var_f9ae9163_checksum_str); // Runs the built-in SHA3-512 implementation and returns its success state.
    } // Ends SHA3-512 dispatch.
    return false; // Reports failure when the requested algorithm is unsupported.
} // Ends algorithm normalization and dispatch.
