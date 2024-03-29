//--------------------------------------------------//
// Headers
//--------------------------------------------------//
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

//--------------------------------------------------//
// Definitions
//--------------------------------------------------//
#define __RHEA_VERSION__ "0.1.0-alpha.9"
#define __RHEA_DEBUG__ 0

//--------------------------------------------------//
// Code
//--------------------------------------------------//
namespace Rhea {
    /**
     * @brief Converts a value into its hexadecimal representation.
     * @param value The value.
     * @param digits The digits.
     * @return The hexadecimal representation.
     */
    static std::string format_hex(const std::uint16_t value, const std::uint8_t digits = 4) {
        std::stringstream converter;

        converter << std::setfill('0') << std::setw(digits) << std::uppercase << std::hex << value;

        return converter.str();
    }

    /**
     * @brief Checks if a string is prefixed.
     * @param string The string.
     * @param prefix The prefix.
     * @return Whether or not is the string prefixed.
     */
    static bool is_prefixed(const std::string string, const std::string prefix) {
        return string.size() >= prefix.size() && 0 == string.compare(0, prefix.size(), prefix);
    }

    /**
     * @brief Checks if a string is suffixed.
     * @param string The string.
     * @param suffix The suffix.
     * @return Whether or not is the string suffixed.
     */
    static bool is_suffixed(const std::string string, const std::string suffix) {
        return string.size() >= suffix.size() && 0 == string.compare(string.size() - suffix.size(), suffix.size(), suffix);
    }

    /**
     * @brief Splits a string into tokens.
     * @param string The string.
     * @param delimiter The delimiter
     * @return The tokens.
     * @note Solution from https://stackoverflow.com/questions/14265581.
     */
    static std::vector<std::string> split(const std::string string, const std::string delimiter) {
        std::size_t start = 0, end = 0, size = delimiter.size();
        std::string token;
        std::vector<std::string> tokens;

        while((end = string.find(delimiter, start)) != std::string::npos) {
            token = string.substr(start, end - start);
            start = end + size;
            tokens.push_back(token);
        }

        tokens.push_back(string.substr(start));

        return tokens;
    }

    class Memory {
        public:
            /**
             * @brief Constructor.
             */
            Memory() {
                this->data = new std::uint8_t[0x10000]();
            }

            /**
             * @brief Get a byte from a specific address.
             * @param address The address of the byte.
             * @return The byte
             */
            std::uint8_t get(const std::uint16_t address) const noexcept {
                return this->data[address];
            }

            /**
             * @brief Set a byte from a specific address to a specific value.
             * @param address The address
             * @param value The value.
             * @return Self-reference.
             */
            Memory & set(const std::uint16_t address, const std::uint8_t value) noexcept {
                this->data[address] = value;
                return *this;
            }

            /**
             * @brief Loads the contents of a binary file into the ROM.
             * @param path The path to the file.
             * @return If the operation was successful.
             */
            bool load(const std::string path) {
                std::ifstream file = std::ifstream(path, std::ios::binary);

                if(!file.is_open()) {
                    std::cerr << "\tFile at \"" << path << "\" not found." << std::endl;
                    return false;
                }

                if(!file.read(reinterpret_cast<char *> (&this->data[0x8000]), 0x8000)) {
                    std::cerr << "\tFile at \"" << path << "\" can't be loaded." << std::endl;
                    return false;
                }

                file.close();

                return true;
            }

            /**
             * @brief Saves the contents of the ROM into a binary file.
             * @param path The path to the file.
             * @return If the operation was successful.
             */
            bool save(const std::string path) const {
                std::ofstream file = std::ofstream(path, std::ios::binary);

                if(!file.is_open()) {
                    std::cerr << "\tFile at \"" << path << "\" not found." << std::endl;
                    return false;
                }

                if(!file.write(reinterpret_cast<char *> (&this->data[0x8000]), 0x8000)) {
                    std::cerr << "\tFile at \"" << path << "\" can't be saved." << std::endl;
                    return false;
                }

                file.close();

                return true;
            }

        private:
            /**
             * @brief Internal representation of the memory.
             */
            std::uint8_t * data;
    };

    class Machine {
        public:
            /**
             * @brief Constructor.
             */
            Machine() noexcept {
                this->a_register = 0x00;
                this->x_register = 0x00;
                this->y_register = 0x00;
                this->s_register = 0x00;
                this->i_pointer = 0x8000;
                this->s_pointer = 0xFF;
                this->memory = Memory();
            }

            ~Machine() noexcept {
                this->memory.~Memory();
            }

            /**
             * @brief Loads the contents of a binary file into the ROM.
             * @param path The path to the file.
             * @return If the operation was successful.
             */
            bool load(const std::string path) {
                return this->memory.load(path);
            }

            /**
             * @brief Saves the contents of the ROM into a binary file.
             * @param path The path to the file.
             * @return If the operation was successful.
             */
            bool save(const std::string path) const {
                return this->memory.save(path);
            }

            /**
             * @brief Dumps the state of the machine into a text file. This is useful when some kind of error occurs.
             * @param path The path to the file.
             * @return If the operation was successful.
             */
            bool dump(const std::string path) const {
                std::ofstream file = std::ofstream(path);

                if(!file.is_open()) {
                    std::cerr << "\tFile at \"" << path << "\" not found." << std::endl;
                    return false;
                }

                file << "Registers" << std::endl;
                file << "----------------------------------------------------------------------" << std::endl;
                file << "AR: " << format_hex(this->a_register);
                file << "  ";
                file << "XR: " << format_hex(this->x_register);
                file << "  ";
                file << "YR: " << format_hex(this->y_register);
                file << "  ";
                file << "SR: " << format_hex(this->s_register);
                file << std::endl;
                file << std::endl;
                file << "Pointers" << std::endl;
                file << "----------------------------------------------------------------------" << std::endl;
                file << "IP: " << format_hex(this->i_pointer) << " -> (" << format_hex(this->memory.get(this->i_pointer), 2) << ")";
                file << "  ";
                file << "SP: " << format_hex(this->s_pointer) << " -> (" << format_hex(this->memory.get(0x0100 + this->s_pointer), 2) << ")";
                file << std::endl;
                file << std::endl;
                file << "Memory Contents (Near IP):" << std::endl;
                file << "----------------------------------------------------------------------" << std::endl;

                for(std::uint32_t address = this->i_pointer; address <= (this->i_pointer + 0x00FF);) {
                    std::string hex_view = "";
                    std::string char_view = "";

                    for(std::uint32_t scan_address = address; scan_address <= (address + 0xF); scan_address++) {
                        if(scan_address > 0xFFFF) { // If the currently scanned address is greater than 0xFFFF,
                            hex_view += "   ";      // display
                            char_view += ' ';       // spaces.
                        } else {                    // Else, get the actual value and display it.
                            std::uint8_t value = this->memory.get(scan_address);

                            hex_view += format_hex(value, 2); // Convert the value to his hexadecimal representation and display it.
                            hex_view += ' ';

                            if(!std::isprint(value)) { // If the value is not displayable,
                                char_view += '.';      // display a '.'.
                            }  else {                  // Else, convert the value to his character representation
                                char_view += value;    // and display it.
                            }
                        }
                    }

                    file << hex_view << "\t" << char_view << std::endl; // Display the hexadecimal and character representation of the values on.

                    address += 0x10;

                    if(address > 0xFFFF) { // If the address is greater than 0xFFFF,
                        break;             // stop.
                    }
                }

                return true;
            }

            Memory & reference_memory() noexcept {
                return this->memory;
            }

            std::uint8_t & reference_a_register() noexcept {
                return this->a_register;
            }

            std::uint8_t & reference_x_register() noexcept {
                return this->x_register;
            }

            std::uint8_t & reference_y_register() noexcept {
                return this->y_register;
            }

            std::uint8_t & reference_s_register() noexcept {
                return this->s_register;
            }

            std::uint16_t & reference_i_pointer() noexcept {
                return this->i_pointer;
            }

            std::uint8_t & reference_s_pointer() noexcept{
                return this->s_pointer;
            }

        private:
            //--------------------------------------------------//
            // Registers
            //--------------------------------------------------//

            /**
             * @brief A (Accumulator) Register: The accumulator, one of the most used registers.
             */
            std::uint8_t a_register;

            /**
             * @brief X Register: General purpose register.
             */
            std::uint8_t x_register;

            /**
             * @brief Y Register: General purpose register.
             */
            std::uint8_t y_register;

            /**
             * @brief S (Status) Register: The status, holds the current status of the machine.
             */
            std::uint8_t s_register;

            //--------------------------------------------------//
            // Pointers
            //--------------------------------------------------//

            /**
             * @brief Instruction Pointer: Points to the location of the current instruction in the memory.
             */
            std::uint16_t i_pointer;

            /**
             * @brief Stack Pointer: Points to the location of the current stack value.
             */
            std::uint8_t s_pointer;

            //--------------------------------------------------//
            // Storage
            //--------------------------------------------------//

            /**
             * @brief Memory : Holds all the data, this includes the ROM and the RAM.
             */
            Memory memory;
    };
}


//--------------------------------------------------//
// Main Code
//--------------------------------------------------//
int main(int argc, const char * argv[]) {
    Rhea::Machine machine = Rhea::Machine();

    std::cout << "Rhea [Version: " << __RHEA_VERSION__ << "]" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;

    bool is_option_none, is_option_1, is_option_2, is_option_3, is_option_4, is_option_5, is_option_6, is_option_7, is_option_8;
    std::uint8_t byte_1, byte_2;
    std::uint16_t word_1, word_2;
    std::string command, command_arg_1, command_arg_2, command_arg_3, command_arg_4;
    std::vector<std::string> command_varargs;

    while(true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if(Rhea::is_prefixed(command, "-load")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                command_arg_1 = "rom.bin";

                std::cout << "\tUsing default value \"" << command_arg_1 << "\" of argument #1 of this command." << std::endl;
            } else {
                command_arg_1 = command_varargs.at(1);
            }

            if(machine.load(command_arg_1)) {
                std::cout << "\tLoaded the \"" << command_arg_1 << "\" file correctly." << std::endl;
            }

            continue;
        }

        if(Rhea::is_prefixed(command, "-save")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                command_arg_1 = "rom.bin";

                std::cout << "\tUsing default value \"" << command_arg_1 << "\" of argument #1 of this command." << std::endl;
            } else {
                command_arg_1 = command_varargs.at(1);
            }

            if(machine.save(command_arg_1)) {
                std::cout << "\tSaved the \"" << command_arg_1 << "\" file correctly." << std::endl;
            }

            continue;
        }

        if(Rhea::is_prefixed(command, "-dump")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                command_arg_1 = "machine.dmp";

                std::cout << "\tUsing default value \"" << command_arg_1 << "\" of argument #1 of this command." << std::endl;
            } else {
                command_arg_1 = command_varargs.at(1);
            }

            if(machine.dump(command_arg_1)) {
                std::cout << "\tDumped the machine into the \"" << command_arg_1 << "\" file correctly." << std::endl;
            }

            continue;
        }

        if(Rhea::is_prefixed(command, "-jump")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                command_arg_1 = "0000";

                std::cout << "\tUsing default value \"" << command_arg_1 << "\" of argument #1 of this command." << std::endl;
            } else {
                command_arg_1 = command_varargs.at(1);
            }

            is_option_1 = Rhea::is_prefixed(command_arg_1, "+");
            is_option_2 = Rhea::is_prefixed(command_arg_1, "-");

            try {
                word_1 = std::labs(std::stol(command_arg_1, 0, 16));
            } catch(...) {
                std::cerr << "\tCan't parse argument #1 value \"" << command_arg_1 << "\"." << std::endl;
                continue;
            }

            if(is_option_1 || is_option_2) {
                std::cout << "\tJumped to the \"" << Rhea::format_hex(machine.reference_i_pointer()) << "\"" << (is_option_2 ? " - " : " + ") << "\"" << Rhea::format_hex(word_1) << "\" address successfully." << std::endl;

                if(is_option_1) {
                    machine.reference_i_pointer() += word_1;
                }

                if(is_option_2) {
                    machine.reference_i_pointer() -= word_1;
                }

                continue;
            }

            machine.reference_i_pointer() = word_1;

            std::cout << "\tJumped to the \"" << Rhea::format_hex(word_1) << "\" address successfully." << std::endl;
            continue;
        }

        if(Rhea::is_prefixed(command, "-get")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                command_arg_1 = "0000";

                std::cout << "\tUsing default value \"" << command_arg_1 << "\" of argument #1 of this command." << std::endl;
            } else {
                command_arg_1 = command_varargs.at(1);
            }

            is_option_1 = command_arg_1 == "ar";
            is_option_2 = command_arg_1 == "xr";
            is_option_3 = command_arg_1 == "yr";
            is_option_4 = command_arg_1 == "sr";
            is_option_5 = command_arg_1 == "ip";
            is_option_6 = command_arg_1 == "sp";
            is_option_none = !(is_option_1 || is_option_2 || is_option_3 || is_option_4 || is_option_5 || is_option_6);

            try {
                word_1 = is_option_none ? std::labs(std::stol(command_arg_1, 0, 16)) : 0x0000;
            } catch(...) {
                std::cerr << "\tCan't parse argument #1 value \"" << command_arg_1 << "\"." << std::endl;
                continue;
            }

            word_2 = is_option_none ? machine.reference_memory().get(word_1) : 0x0000;

            if(!is_option_none) {
                if(is_option_1) {
                    word_2 = machine.reference_a_register();
                }

                if(is_option_2) {
                    word_2 = machine.reference_x_register();
                }

                if(is_option_3) {
                    word_2 = machine.reference_y_register();
                }

                if(is_option_4) {
                    word_2 = machine.reference_s_register();
                }

                if(is_option_5) {
                    word_2 = machine.reference_i_pointer();
                }

                if(is_option_6) {
                    word_2 = machine.reference_s_pointer();
                }

                std::cout << "\tThe value of \"" << command_arg_1 << "\" is \"" << Rhea::format_hex(word_2, (is_option_5 ? 4 : 2)) << "\"." << std::endl;
                continue;
            }

            std::cout << "\tThe value on the \"" << Rhea::format_hex(word_1) << "\" address is \"" << Rhea::format_hex(word_2, 2) << "\"." << std::endl;
            continue;
        }

        if(Rhea::is_prefixed(command, "-set")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                command_arg_1 = "0000";

                std::cout << "\tUsing default value \"" << command_arg_1 << "\" of argument #1 of this command." << std::endl;
            } else {
                command_arg_1 = command_varargs.at(1);
            }

            if(command_varargs.size() < 3) {
                command_arg_2 = "00";

                std::cout << "\tUsing default value \"" << command_arg_2 << "\" of argument #2 of this command." << std::endl;
            } else {
                command_arg_2 = command_varargs.at(2);
            }

            is_option_1 = command_arg_1 == "ar";
            is_option_2 = command_arg_1 == "xr";
            is_option_3 = command_arg_1 == "yr";
            is_option_4 = command_arg_1 == "sr";
            is_option_5 = command_arg_1 == "ip";
            is_option_6 = command_arg_1 == "sp";
            is_option_none = !(is_option_1 || is_option_2 || is_option_3 || is_option_4 || is_option_5 || is_option_6);

            try {
                word_1 = is_option_none ? std::labs(std::stol(command_arg_1, 0, 16)) : 0x0000;
            } catch(...) {
                std::cerr << "\tCan't parse argument #1 value \"" << command_arg_1 << "\"." << std::endl;
                continue;
            }
            
            try {
                word_2 = std::labs(std::stol(command_arg_2, 0, 16));
            } catch(...) {
                std::cerr << "\tCan't parse argument #2 value \"" << command_arg_2 << "\"." << std::endl;
                continue;
            }

            if(!is_option_none) {
                if(!is_option_5) {
                    if(word_2 > 0xFF) {
                        std::cerr << "\tArgument #2 is out of range." << std::endl;
                        continue;
                    }

                    if(is_option_1) {
                        machine.reference_a_register() = word_2;
                    }

                    if(is_option_2) {
                        machine.reference_x_register() = word_2;
                    }

                    if(is_option_3) {
                        machine.reference_y_register() = word_2;
                    }

                    if(is_option_4) {
                        machine.reference_s_register() = word_2;
                    }

                    if(is_option_6) {
                        machine.reference_s_pointer() = word_2;
                    }
                }

                machine.reference_i_pointer() = word_2;

                std::cout << "\tThe value of \"" << command_arg_1 << "\" is now \"" << Rhea::format_hex(word_2, (is_option_5 ? 4 : 2)) << "\"." << std::endl;
                continue;
            }

            machine.reference_memory().set(word_1, word_2);

            std::cout << "\tThe value on the \"" << Rhea::format_hex(word_1) << "\" address is now \"" << Rhea::format_hex(word_2, 2) << "\"." << std::endl;
            continue;
        }

        if(Rhea::is_prefixed(command, "-help")) {
            command_varargs = Rhea::split(command, " ");

            if(command_varargs.size() < 2) {
                std::cerr << "\tCommand name required as argument #1." << std::endl;
                continue;
            } else {
                command_arg_1 = command_varargs.at(1);
            }
        
            is_option_1 = command_arg_1 == "dump";
            is_option_2 = command_arg_1 == "get";
            is_option_3 = command_arg_1 == "jump";
            is_option_4 = command_arg_1 == "load";
            is_option_5 = command_arg_1 == "save";
            is_option_6 = command_arg_1 == "set";
            
            if(is_option_1) {
                std::cout << "\tdump <file : string>"<< std::endl;
                std::cout << std::endl;
                std::cout << "\tDumps the state of the machine into a text \"file\". This is just the" << std::endl;
                std::cout << "\tcurrent values of the AR, XR, YR and SR; anlog with the values that IP" << std::endl;
                std::cout << "\tand SP currently point to. Also a view to the next 255 bytes after the" << std::endl;
                std::cout << "\tvalue IP is currently pointing to is provided (including IP)." << std::endl;
                continue;
            }

            if(is_option_2) {
                std::cout << "\tget <address : number>"<< std::endl;
                std::cout << "\tget <internal : string (ar | xr | yr | sr | ip | sp)>"<< std::endl;
                std::cout << std::endl;
                std::cout << "\tGets the value at the specified \"address\" on the memory or at the" << std::endl;
                std::cout << "\tspecified \"internal\" register/pointer. The value can be a byte or a" << std::endl;
                std::cout << "\tword depending of the register/pointer, but with the addresses it will" << std::endl;
                std::cout << "\talways be a byte." << std::endl;
                continue;
            }

            if(is_option_3) {
                std::cout << "\tjump [sign: string (+ | -)]<address: number>"<< std::endl;
                std::cout << std::endl;
                std::cout << "\tJumps (sets IP) to an specific \"address\" in the memory. If the \"sign\"" << std::endl;
                std::cout << "\tis + the machine jumps to [IP + \"address\"]; else, if it's - the machine" << std::endl;
                std::cout << "\tjumps to [IP - \"address\"]. If there is no \"sign\" the machine just" << std::endl;
                std::cout << "\tjumps to the \"address\"." << std::endl;
                continue;
            }

            if(is_option_4) {
                std::cout << "\tload <file : string>"<< std::endl;
                std::cout << std::endl;
                std::cout << "\tLoads the ROM from a binary \"file\" of 32 KB." << std::endl;
                continue;
            }

            if(is_option_4) {
                std::cout << "\tload <file : string>"<< std::endl;
                std::cout << std::endl;
                std::cout << "\tSaves the ROM into a binary \"file\" of 32 KB." << std::endl;
                continue;
            }

            if(is_option_6) {
                std::cout << "\tset <address : number> <value: number>"<< std::endl;
                std::cout << "\tset <internal : string (ar | xr | yr | sr | ip | sp)> <value: number>"<< std::endl;
                std::cout << std::endl;
                std::cout << "\tSets the \"value\" at the specified \"address\" on the memory or at the" << std::endl;
                std::cout << "\tspecified \"internal\" register/pointer. The \"value\" can be a byte or a" << std::endl;
                std::cout << "\tword depending of the register/pointer, but with the addresses it will" << std::endl;
                std::cout << "\talways be a byte." << std::endl;
                continue;
            }

            std::cerr << "\tUnrecognized help topic \"" << command_arg_1 << "\"." << std::endl;
            continue;
        }

        if(Rhea::is_prefixed(command, "-exit") || Rhea::is_prefixed(command, "-quit")) {
            break;
        }

        std::cerr << "\tUnrecognized command \"" << command << "\"." << std::endl;
    }

    return 0;
}