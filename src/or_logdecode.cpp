#include <iostream>
#include "strings.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>
#include "readlog.h"

std::string strings::syntax = "syntax: orlogdecode.exe OpenRailsLog.txt [options]\n";
std::string strings::help_en = "Open Rails logfile decoder 0.1\n" +
                        syntax +
                        "options : \n" +
                        "-h --help: shows this menu\n" +
                        "-v --version: shows version info\n" +
                        "-logall: logging into the console (not implemented)\n";
std::string strings::nofile_en = "no file specified\n use -h for help";
std::string strings::version = "Open Rails Logfile decoder version 0.1";


void cli_interface(const std::string& errors, const std::string& warnings,
    const std::string& info, const std::string& header) {
    std::string command;

    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command == "err") {
            std::cout << "Errors:\n" << (errors.empty() ? "No errors found.\n" : errors) << std::endl;
        }
        else if (command == "warn") {
            std::cout << "Warnings:\n" << (warnings.empty() ? "No warnings found.\n" : warnings) << std::endl;
        }
        else if (command == "info") {
            std::cout << "Info:\n" << (info.empty() ? "No info found.\n" : info) << std::endl;
        }
        else if (command == "head") {
            std::cout << "Header:\n" << (header.empty() ? "No header found.\n" : header) << std::endl;
        }
        else if (command == "help") std::cout << "head, info, warn, err, write, help, exit" << std::endl;
        else if (command == "write") {
            std::cout << "which message type do you want to write?> ";
            std::string segment;
            std::cin >> segment;

            std::string filename;
            std::cout << "enter filename: ";
            std::cin >> filename;

            std::ofstream out_file(filename);
            if (!out_file) {
                std::cerr << "cannot open file for writing: " << filename << std::endl;
                std::perror("std::ofstream");
                continue;
            }

            if (segment == "err") {
                if (errors.empty()) {
                    std::cout << "no errors to write.\n";
                }
                else {
                    out_file << errors;
                    std::cout << "errors written to " << filename << std::endl;
                }
            }
            else if (segment == "warn") {
                if (warnings.empty()) {
                    std::cout << "no warnings to write.\n";
                }
                else {
                    out_file << warnings;
                    std::cout << "warnings written to " << filename << std::endl;
                }
            }
            else if (segment == "info") {
                if (info.empty()) {
                    std::cout << "no info to write.\n";
                }
                else {
                    out_file << info;
                    std::cout << "info written to " << filename << std::endl;
                }
            }
            else if (segment == "head") {
                if (header.empty()) {
                    std::cout << "no header to write.\n";
                }
                else {
                    out_file << header;
                    std::cout << "header written to " << filename << std::endl;
                }
            }
            else {
                std::cerr << "unknown segment: " << segment << std::endl;
            }

            out_file.close();
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cerr << "unknown command: " << command << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    bool logall = false;
    if (argc < 2)
    {
        std::cout << strings::nofile_en << std::endl;
        return 1;
    }
    else if ((argc == 2) && !strcmp("-v", argv[1]) || (argc == 1) && !strcmp("--version", argv[1]))
    {
        std::cout << strings::version << std::endl;
        return 0;
    }
    else if ((argc == 2) && !strcmp("-h", argv[1]) || (argc == 2) && !strcmp("--help", argv[1])) {
        std::cout << strings::help_en << std::endl;
        return 0;
    }
    else if ((argc == 3) && !strcmp("-logall", argv[2])) {
        logall = true;
    }

    if(logall) std::cout << "current working directory: " << std::filesystem::current_path() << std::endl;
    
    std::string file_path = argv[1];
    std::string content = slurp_file(file_path, logall);

    if (content.empty()) {
        std::cerr << "file is empty" << std::endl;
        return 1;
    }

    std::string errors, warnings, info, header;
    extract_segments(content, errors, warnings, info, header, logall);
    if (logall) std::cout << "logfile segmented" << std::endl;

    if (logall) std::cout << "starting cli" << std::endl;
    cli_interface(errors, warnings, info, header);
    return 0;
}

