// Description: Implements SHA-512 directly in C++ and hashes bytes from a stream without external hashing libraries.
#include "hash_sha512.hpp" // Imports the SHA-512 function declaration.
#include <array> // Provides fixed-size arrays for state, constants, and the message schedule.
#include <cstddef> // Provides std::size_t and std::ptrdiff_t.
#include <cstdint> // Provides fixed-width unsigned integer types.
#include <iomanip> // Provides hexadecimal width and fill formatting.
#include <sstream> // Provides an output string stream for hexadecimal conversion.
#include <vector> // Provides a small pending-byte buffer used across stream reads.
bool fun_a96b797b_hash_sha512_stream_i1790544726_v1(std::istream& var_a96b797b_input_stream_obj, std::string& var_a96b797b_checksum_str) { // Starts SHA-512 stream hashing.
    std::array<std::uint64_t, 8> var_a96b797b_state_arr = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; // Initializes the eight standard SHA-512 chaining values.
    const std::array<std::uint64_t, 80> var_a96b797b_constants_arr = { // Stores the eighty SHA-512 round constants.
        0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, // Stores constants 0 through 7.
        0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, // Stores constants 8 through 15.
        0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, // Stores constants 16 through 23.
        0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, // Stores constants 24 through 31.
        0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, // Stores constants 32 through 39.
        0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, // Stores constants 40 through 47.
        0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, // Stores constants 48 through 55.
        0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, // Stores constants 56 through 63.
        0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, // Stores constants 64 through 71.
        0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL // Stores constants 72 through 79.
    }; // Ends the SHA-512 constants array.
    std::uint64_t var_a96b797b_total_bytes_num = 0ULL; // Counts original source bytes for the 128-bit final length field.
    std::vector<std::uint8_t> var_a96b797b_pending_arr; // Stores bytes that do not yet form a complete 128-byte block.
    var_a96b797b_pending_arr.reserve(8320U); // Reserves room for one source chunk plus a partial SHA-512 block.
    auto var_a96b797b_process_block_obj = [&](const std::uint8_t* var_a96b797b_block_arr) { // Defines localized processing for one 1024-bit SHA-512 block.
        std::array<std::uint64_t, 80> var_a96b797b_schedule_arr{}; // Allocates the eighty-word SHA-512 message schedule.
        for (std::size_t var_a96b797b_word_index_num = 0U; var_a96b797b_word_index_num < 16U; ++var_a96b797b_word_index_num) { // Decodes the sixteen words stored directly in the input block.
            std::size_t var_a96b797b_offset_num = var_a96b797b_word_index_num * 8U; // Calculates the byte offset of the current 64-bit word.
            var_a96b797b_schedule_arr[var_a96b797b_word_index_num] = (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num]) << 56U) | (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 1U]) << 48U) | (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 2U]) << 40U) | (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 3U]) << 32U) | (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 4U]) << 24U) | (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 5U]) << 16U) | (static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 6U]) << 8U) | static_cast<std::uint64_t>(var_a96b797b_block_arr[var_a96b797b_offset_num + 7U]); // Decodes one big-endian 64-bit message word.
        } // Ends input-word decoding.
        for (std::size_t var_a96b797b_word_index_num = 16U; var_a96b797b_word_index_num < 80U; ++var_a96b797b_word_index_num) { // Expands the initial sixteen words into the complete eighty-word schedule.
            std::uint64_t var_a96b797b_previous15_num = var_a96b797b_schedule_arr[var_a96b797b_word_index_num - 15U]; // Loads the word used by small sigma zero.
            std::uint64_t var_a96b797b_previous2_num = var_a96b797b_schedule_arr[var_a96b797b_word_index_num - 2U]; // Loads the word used by small sigma one.
            std::uint64_t var_a96b797b_sigma0_num = ((var_a96b797b_previous15_num >> 1U) | (var_a96b797b_previous15_num << 63U)) ^ ((var_a96b797b_previous15_num >> 8U) | (var_a96b797b_previous15_num << 56U)) ^ (var_a96b797b_previous15_num >> 7U); // Calculates SHA-512 small sigma zero.
            std::uint64_t var_a96b797b_sigma1_num = ((var_a96b797b_previous2_num >> 19U) | (var_a96b797b_previous2_num << 45U)) ^ ((var_a96b797b_previous2_num >> 61U) | (var_a96b797b_previous2_num << 3U)) ^ (var_a96b797b_previous2_num >> 6U); // Calculates SHA-512 small sigma one.
            var_a96b797b_schedule_arr[var_a96b797b_word_index_num] = var_a96b797b_schedule_arr[var_a96b797b_word_index_num - 16U] + var_a96b797b_sigma0_num + var_a96b797b_schedule_arr[var_a96b797b_word_index_num - 7U] + var_a96b797b_sigma1_num; // Produces the next schedule word modulo 2^64.
        } // Ends schedule expansion.
        std::uint64_t var_a96b797b_a_num = var_a96b797b_state_arr[0]; // Copies chaining value zero into working register A.
        std::uint64_t var_a96b797b_b_num = var_a96b797b_state_arr[1]; // Copies chaining value one into working register B.
        std::uint64_t var_a96b797b_c_num = var_a96b797b_state_arr[2]; // Copies chaining value two into working register C.
        std::uint64_t var_a96b797b_d_num = var_a96b797b_state_arr[3]; // Copies chaining value three into working register D.
        std::uint64_t var_a96b797b_e_num = var_a96b797b_state_arr[4]; // Copies chaining value four into working register E.
        std::uint64_t var_a96b797b_f_num = var_a96b797b_state_arr[5]; // Copies chaining value five into working register F.
        std::uint64_t var_a96b797b_g_num = var_a96b797b_state_arr[6]; // Copies chaining value six into working register G.
        std::uint64_t var_a96b797b_h_num = var_a96b797b_state_arr[7]; // Copies chaining value seven into working register H.
        for (std::size_t var_a96b797b_round_index_num = 0U; var_a96b797b_round_index_num < 80U; ++var_a96b797b_round_index_num) { // Performs all eighty SHA-512 compression rounds.
            std::uint64_t var_a96b797b_big_sigma1_num = ((var_a96b797b_e_num >> 14U) | (var_a96b797b_e_num << 50U)) ^ ((var_a96b797b_e_num >> 18U) | (var_a96b797b_e_num << 46U)) ^ ((var_a96b797b_e_num >> 41U) | (var_a96b797b_e_num << 23U)); // Calculates SHA-512 big sigma one from E.
            std::uint64_t var_a96b797b_choose_num = (var_a96b797b_e_num & var_a96b797b_f_num) ^ ((~var_a96b797b_e_num) & var_a96b797b_g_num); // Calculates the SHA-512 choose function.
            std::uint64_t var_a96b797b_temporary1_num = var_a96b797b_h_num + var_a96b797b_big_sigma1_num + var_a96b797b_choose_num + var_a96b797b_constants_arr[var_a96b797b_round_index_num] + var_a96b797b_schedule_arr[var_a96b797b_round_index_num]; // Computes temporary value one modulo 2^64.
            std::uint64_t var_a96b797b_big_sigma0_num = ((var_a96b797b_a_num >> 28U) | (var_a96b797b_a_num << 36U)) ^ ((var_a96b797b_a_num >> 34U) | (var_a96b797b_a_num << 30U)) ^ ((var_a96b797b_a_num >> 39U) | (var_a96b797b_a_num << 25U)); // Calculates SHA-512 big sigma zero from A.
            std::uint64_t var_a96b797b_majority_num = (var_a96b797b_a_num & var_a96b797b_b_num) ^ (var_a96b797b_a_num & var_a96b797b_c_num) ^ (var_a96b797b_b_num & var_a96b797b_c_num); // Calculates the SHA-512 majority function.
            std::uint64_t var_a96b797b_temporary2_num = var_a96b797b_big_sigma0_num + var_a96b797b_majority_num; // Computes temporary value two modulo 2^64.
            var_a96b797b_h_num = var_a96b797b_g_num; // Rotates working register G into H.
            var_a96b797b_g_num = var_a96b797b_f_num; // Rotates working register F into G.
            var_a96b797b_f_num = var_a96b797b_e_num; // Rotates working register E into F.
            var_a96b797b_e_num = var_a96b797b_d_num + var_a96b797b_temporary1_num; // Computes the new E value modulo 2^64.
            var_a96b797b_d_num = var_a96b797b_c_num; // Rotates working register C into D.
            var_a96b797b_c_num = var_a96b797b_b_num; // Rotates working register B into C.
            var_a96b797b_b_num = var_a96b797b_a_num; // Rotates working register A into B.
            var_a96b797b_a_num = var_a96b797b_temporary1_num + var_a96b797b_temporary2_num; // Computes the new A value modulo 2^64.
        } // Ends compression rounds.
        var_a96b797b_state_arr[0] += var_a96b797b_a_num; // Adds working register A back into chaining value zero.
        var_a96b797b_state_arr[1] += var_a96b797b_b_num; // Adds working register B back into chaining value one.
        var_a96b797b_state_arr[2] += var_a96b797b_c_num; // Adds working register C back into chaining value two.
        var_a96b797b_state_arr[3] += var_a96b797b_d_num; // Adds working register D back into chaining value three.
        var_a96b797b_state_arr[4] += var_a96b797b_e_num; // Adds working register E back into chaining value four.
        var_a96b797b_state_arr[5] += var_a96b797b_f_num; // Adds working register F back into chaining value five.
        var_a96b797b_state_arr[6] += var_a96b797b_g_num; // Adds working register G back into chaining value six.
        var_a96b797b_state_arr[7] += var_a96b797b_h_num; // Adds working register H back into chaining value seven.
    }; // Ends localized SHA-512 block processing.
    char var_a96b797b_buffer_arr[8192]; // Allocates a fixed-size source read buffer.
    while (true) { // Repeats source reads until explicit end-of-file or an I/O error is detected.
        var_a96b797b_input_stream_obj.read(var_a96b797b_buffer_arr, sizeof(var_a96b797b_buffer_arr)); // Attempts to read the next fixed-size source chunk without relying on implicit stream truthiness.
        std::streamsize var_a96b797b_bytes_read_num = var_a96b797b_input_stream_obj.gcount(); // Records how many bytes were actually read into the current source chunk.
        if (var_a96b797b_bytes_read_num <= 0) { // Checks whether the read produced no bytes that need hashing.
            if (var_a96b797b_input_stream_obj.eof() == true) { // Checks explicitly for normal end-of-file.
                break; // Stops source streaming after all bytes have been processed.
            } // Ends explicit end-of-file handling.
            if ((var_a96b797b_input_stream_obj.bad() == true) || (var_a96b797b_input_stream_obj.fail() == true)) { // Checks explicitly for a non-EOF stream failure.
                return false; // Reports failure rather than hashing incomplete source data.
            } // Ends explicit stream-failure handling.
            continue; // Retries only if the stream reported neither bytes, EOF, nor a failure state.
        } // Ends no-bytes handling.
        var_a96b797b_total_bytes_num += static_cast<std::uint64_t>(var_a96b797b_bytes_read_num); // Adds the current chunk size to the original message length.
        for (std::streamsize var_a96b797b_copy_index_num = 0; var_a96b797b_copy_index_num < var_a96b797b_bytes_read_num; ++var_a96b797b_copy_index_num) { // Copies the current source chunk into the pending-byte buffer.
            var_a96b797b_pending_arr.push_back(static_cast<std::uint8_t>(var_a96b797b_buffer_arr[var_a96b797b_copy_index_num])); // Preserves each source byte exactly.
        } // Ends current-chunk copying.
        std::size_t var_a96b797b_processed_bytes_num = 0U; // Tracks complete pending bytes processed during this read cycle.
        while ((var_a96b797b_pending_arr.size() - var_a96b797b_processed_bytes_num) >= 128U) { // Processes every complete 128-byte SHA-512 block.
            var_a96b797b_process_block_obj(var_a96b797b_pending_arr.data() + var_a96b797b_processed_bytes_num); // Transforms the next complete block.
            var_a96b797b_processed_bytes_num += 128U; // Advances past the transformed block.
        } // Ends complete-block processing.
        if (var_a96b797b_processed_bytes_num > 0U) { // Checks whether processed bytes can be removed from the pending buffer.
            var_a96b797b_pending_arr.erase(var_a96b797b_pending_arr.begin(), var_a96b797b_pending_arr.begin() + static_cast<std::ptrdiff_t>(var_a96b797b_processed_bytes_num)); // Retains only the incomplete tail for the next read.
        } // Ends pending-buffer compaction.
    } // Ends source streaming.
    if (var_a96b797b_input_stream_obj.bad() == true) { // Checks whether a genuine source I/O failure occurred.
        return false; // Reports failure rather than returning a digest for incomplete source data.
    } // Ends source error checking.
    std::uint64_t var_a96b797b_length_high_num = var_a96b797b_total_bytes_num >> 61U; // Calculates the high 64 bits of the 128-bit original bit length.
    std::uint64_t var_a96b797b_length_low_num = var_a96b797b_total_bytes_num << 3U; // Calculates the low 64 bits of the 128-bit original bit length.
    var_a96b797b_pending_arr.push_back(0x80U); // Appends the mandatory one bit followed by seven zero bits.
    while ((var_a96b797b_pending_arr.size() % 128U) != 112U) { // Pads with zeros until sixteen bytes remain for the 128-bit length field.
        var_a96b797b_pending_arr.push_back(0x00U); // Appends one zero-padding byte.
    } // Ends zero padding.
    for (int var_a96b797b_length_index_num = 7; var_a96b797b_length_index_num >= 0; --var_a96b797b_length_index_num) { // Appends the high half of the 128-bit length in big-endian order.
        var_a96b797b_pending_arr.push_back(static_cast<std::uint8_t>((var_a96b797b_length_high_num >> (8U * static_cast<std::uint32_t>(var_a96b797b_length_index_num))) & 0xffULL)); // Stores one byte of the high length half.
    } // Ends high-length serialization.
    for (int var_a96b797b_length_index_num = 7; var_a96b797b_length_index_num >= 0; --var_a96b797b_length_index_num) { // Appends the low half of the 128-bit length in big-endian order.
        var_a96b797b_pending_arr.push_back(static_cast<std::uint8_t>((var_a96b797b_length_low_num >> (8U * static_cast<std::uint32_t>(var_a96b797b_length_index_num))) & 0xffULL)); // Stores one byte of the low length half.
    } // Ends low-length serialization.
    for (std::size_t var_a96b797b_final_offset_num = 0U; var_a96b797b_final_offset_num < var_a96b797b_pending_arr.size(); var_a96b797b_final_offset_num += 128U) { // Processes all final padded blocks.
        var_a96b797b_process_block_obj(var_a96b797b_pending_arr.data() + var_a96b797b_final_offset_num); // Transforms one final SHA-512 block.
    } // Ends final padded block processing.
    std::ostringstream var_a96b797b_hex_stream_obj; // Creates a stream for lowercase hexadecimal digest output.
    var_a96b797b_hex_stream_obj << std::hex << std::nouppercase << std::setfill('0'); // Configures lowercase hexadecimal encoding with zero padding.
    for (std::size_t var_a96b797b_state_index_num = 0U; var_a96b797b_state_index_num < var_a96b797b_state_arr.size(); ++var_a96b797b_state_index_num) { // Serializes all eight 64-bit chaining values.
        var_a96b797b_hex_stream_obj << std::setw(16) << var_a96b797b_state_arr[var_a96b797b_state_index_num]; // Writes exactly sixteen hexadecimal characters for the current state word.
    } // Ends digest serialization.
    var_a96b797b_checksum_str = var_a96b797b_hex_stream_obj.str(); // Returns the final 512-bit SHA-512 digest as hexadecimal text.
    return true; // Reports successful SHA-512 hashing.
} // Ends SHA-512 stream hashing.
