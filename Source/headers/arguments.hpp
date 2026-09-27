#pragma once // Prevents this header from being included more than once in a translation unit.
#include <string> // Provides std::string for argument values.
// Description: Identifies which input source should be hashed after command-line precedence is resolved.
enum class InputSourceType { // Defines the available input source states without adding behavior.
    None, // Indicates that no enabled input source has been selected.
    SourceFile, // Indicates that bytes should be read from a file path.
    InputString, // Indicates that bytes should be read from a command-line string.
    StandardInput // Indicates that bytes should be read from standard input.
}; // Ends the input source enumeration.
// Description: Stores parsed command-line values used by the application, including project-defined defaults.
struct ArgumentValues { // Groups command-line data without object-oriented methods.
    std::string var_ba1a60f9_algorithm_str = "sha3-512"; // Stores the requested hashing algorithm and defaults to SHA3-512 when algorithm is omitted.
    std::string var_ba1a60f9_format_str = "hexadecimal"; // Stores the requested checksum representation and defaults to hexadecimal when format is omitted.
    std::string var_ba1a60f9_delimiter_str = "  "; // Stores the checksum-file field delimiter and defaults to the two spaces used by compatible checksum tools.
    std::string var_ba1a60f9_source_value_str; // Stores a source file path or input string when applicable.
    std::string var_ba1a60f9_destination_file_str; // Stores the optional checksum output path.
    InputSourceType var_ba1a60f9_source_type_uns = InputSourceType::None; // Stores the last enabled input source selected by command-line order.
    bool var_ba1a60f9_destination_provided_boo = false; // Records whether destination_file was explicitly provided.
    bool var_ba1a60f9_overwrite_boo = false; // Defaults overwrite permission to false unless explicitly enabled with the value true.
}; // Ends the argument value structure.
// Description: Parses name-value argument pairs, applies defaults, and resolves the last enabled input source.
bool fun_fb513b28_parse_arguments_i1790544721_v3(int, char*[], ArgumentValues&); // Declares the command-line parsing function.
