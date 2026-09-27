// Description: THIS FILE WRITES FILES. It writes one checksum-file record only to a path explicitly requested by destination_file and enforces no-clobber behavior unless overwrite is exactly true.
#include "checksum_writer.hpp" // Imports the checksum writer declaration.
#include <cerrno> // Provides errno and EINTR for reliable POSIX write handling.
#include <cstddef> // Provides std::size_t.
#include <fcntl.h> // Provides POSIX open flags including O_EXCL and O_TRUNC.
#include <string> // Provides std::string for the requested path, checksum bytes/text, source label, delimiter, and output record.
#include <sys/stat.h> // Provides the mode type used when creating a destination file.
#include <unistd.h> // Provides POSIX open-file descriptor write and close operations.
bool fun_958a9821_write_checksum_i1790544730_v3(const std::string& var_958a9821_destination_path_str, const std::string& var_958a9821_checksum_str, const std::string& var_958a9821_source_label_str, const std::string& var_958a9821_delimiter_str, bool var_958a9821_overwrite_boo, bool var_958a9821_gnu_filename_escaping_boo) { // Starts the isolated checksum-file writing operation.
    if (var_958a9821_destination_path_str.empty() == true) { // Rejects an empty requested path before any filesystem write is attempted.
        return false; // Reports failure because an empty destination does not identify a writable file.
    } // Ends the empty-path safety check.
    if (var_958a9821_source_label_str.empty() == true) { // Rejects an empty source label because checksum records require a source field.
        return false; // Reports failure instead of creating a malformed checksum-file record.
    } // Ends empty-source-label validation.
    std::string var_958a9821_output_source_str = var_958a9821_source_label_str; // Starts with the exact source label and changes it only when GNU-compatible filename escaping is requested.
    bool var_958a9821_escape_required_boo = false; // Tracks whether GNU-style checksum filename escaping is needed for the default compatible hexadecimal record layout.
    if (var_958a9821_gnu_filename_escaping_boo == true) { // Checks whether the caller requested the filename escaping rules used by compatible GNU checksum records.
        var_958a9821_output_source_str.clear(); // Clears the direct copy before constructing the escaped source field.
        for (std::size_t var_958a9821_index_num = 0U; var_958a9821_index_num < var_958a9821_source_label_str.size(); ++var_958a9821_index_num) { // Visits every source-label byte so backslashes and line breaks can be represented safely.
            char var_958a9821_character_uns = var_958a9821_source_label_str[var_958a9821_index_num]; // Reads the current source-label character.
            if (var_958a9821_character_uns == '\\') { // Checks whether the filename contains a backslash that GNU checksum tools escape.
                var_958a9821_output_source_str += "\\\\"; // Stores the filename backslash using the conventional doubled-backslash escape.
                var_958a9821_escape_required_boo = true; // Records that the checksum record needs the leading escape marker.
            } else if (var_958a9821_character_uns == '\n') { // Checks whether the filename contains a newline that cannot remain literal in a line-based checksum file.
                var_958a9821_output_source_str += "\\n"; // Stores the newline using the conventional backslash-n checksum filename escape.
                var_958a9821_escape_required_boo = true; // Records that the checksum record needs the leading escape marker.
            } else { // Handles ordinary filename characters and the standard dash marker.
                var_958a9821_output_source_str.push_back(var_958a9821_character_uns); // Copies the current source character without modification.
            } // Ends checksum source-label escaping.
        } // Ends source-label scanning.
    } // Ends optional GNU filename escaping.
    std::string var_958a9821_output_str; // Creates binary-safe storage for the complete checksum-file record.
    if (var_958a9821_escape_required_boo == true) { // Checks whether the source field contains GNU checksum-tool escape sequences.
        var_958a9821_output_str.push_back('\\'); // Adds the conventional leading backslash that tells compatible checksum tools the filename field is escaped.
    } // Ends leading escape-marker handling.
    var_958a9821_output_str.append(var_958a9821_checksum_str.data(), var_958a9821_checksum_str.size()); // Adds every checksum byte exactly, preserving embedded NUL values when raw binary format is selected.
    var_958a9821_output_str.append(var_958a9821_delimiter_str.data(), var_958a9821_delimiter_str.size()); // Adds the exact requested checksum-file delimiter, including the default compatible two spaces.
    var_958a9821_output_str.append(var_958a9821_output_source_str.data(), var_958a9821_output_source_str.size()); // Adds the source filename or conventional dash marker after the requested delimiter.
    var_958a9821_output_str.push_back('\n'); // Terminates the checksum-file record with one newline after the source field.
    int var_958a9821_open_flags_num = O_WRONLY | O_CREAT; // Begins with flags that create the requested file if needed and open it only for writing.
    if (var_958a9821_overwrite_boo == true) { // Checks whether the caller explicitly enabled overwriting.
        var_958a9821_open_flags_num |= O_TRUNC; // Permits an existing requested file to be truncated only when overwrite was explicitly enabled.
    } else { // Handles the default protected behavior where existing files must remain untouched.
        var_958a9821_open_flags_num |= O_EXCL; // Makes file creation fail atomically if the requested path already exists, avoiding a check-then-write race.
    } // Ends overwrite-policy flag selection.
    int var_958a9821_file_descriptor_num = ::open(var_958a9821_destination_path_str.c_str(), var_958a9821_open_flags_num, static_cast<mode_t>(0666)); // Opens or creates exactly the user-requested path while respecting the process umask and selected overwrite policy.
    if (var_958a9821_file_descriptor_num < 0) { // Checks whether the operating system refused or failed the requested file open.
        return false; // Reports failure without attempting any write when the destination could not be opened safely.
    } // Ends destination-open validation.
    std::size_t var_958a9821_written_bytes_num = 0U; // Tracks how many payload bytes have already been successfully written.
    while (var_958a9821_written_bytes_num < var_958a9821_output_str.size()) { // Continues until every payload byte has been written or an unrecoverable error occurs.
        ssize_t var_958a9821_write_result_num = ::write(var_958a9821_file_descriptor_num, var_958a9821_output_str.data() + var_958a9821_written_bytes_num, var_958a9821_output_str.size() - var_958a9821_written_bytes_num); // Requests writing of all bytes that remain after any prior partial write.
        if (var_958a9821_write_result_num > 0) { // Checks whether the operating system successfully wrote one or more bytes.
            var_958a9821_written_bytes_num += static_cast<std::size_t>(var_958a9821_write_result_num); // Advances the completed-byte count by the number actually written.
            continue; // Attempts the next remaining portion of the payload if any bytes are still unwritten.
        } // Ends successful partial-write handling.
        if ((var_958a9821_write_result_num < 0) && (errno == EINTR)) { // Checks whether the write was interrupted by a signal before completion.
            continue; // Retries the same unwritten bytes because EINTR does not mean the destination itself failed.
        } // Ends interrupted-write recovery.
        ::close(var_958a9821_file_descriptor_num); // Closes the file descriptor before reporting an unrecoverable write failure.
        return false; // Reports failure because the requested checksum payload could not be fully written.
    } // Ends the complete-payload write loop.
    if (::close(var_958a9821_file_descriptor_num) != 0) { // Closes the destination and checks whether final descriptor closure reported an error.
        return false; // Reports failure if the operating system could not successfully finalize the open descriptor.
    } // Ends close-result validation.
    return true; // Reports success only after the complete checksum-file record has been written and the file descriptor has closed successfully.
} // Ends the isolated checksum-file writing operation.
