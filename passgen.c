/* 
 * passgen.c
 *
 * Copyright 2024 Clay Gomera
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define VERSION 0.1
#define NAME "passgen"

// Function prototypes
char* generatePassword(unsigned int length, bool wantsUpCase, bool wantsLowCase, bool wantsDigits, bool wantsSpecials);
void displayHelp();
void displayVersion();

int main(int argc, char **argv) {
    int option;
    unsigned int length = 24;
    bool wantsUpCase   = false,
         wantsLowCase  = false,
         wantsDigits   = false,
         wantsSpecials = false;

    // Generate a random password if no arguments are given
    if (argc == 1) {
        char* password = generatePassword(length, true, true, true, true);
        printf("Password: %s\n", password);
        return 0;
    }

    // Options
    static const char* short_options = "L:ludshv";
    static struct option long_options[] = {
        {"length",     required_argument, 0, 'L'},
        {"lower-case", no_argument,       0, 'l'},
        {"upper-case", no_argument,       0, 'u'},
        {"digits",     no_argument,       0, 'd'},
        {"special",    no_argument,       0, 's'},
        {"help",       no_argument,       0, 'h'},
        {"version",    no_argument,       0, 'v'},
        {NULL, 0, NULL, 0}
    };

    while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (option) {
            case 'L':
                if ((length = atoi(optarg)) != 0) {
                    if (length > 4096 || length < 5) {
                        fprintf(stderr, "Error: The password length can't be greater than 4096 or less than 5 characters.\n\n");
                        displayHelp();
                        return 1;
                    }
                }
                else {
                    fprintf(stderr, "Invalid value provided. Please provide a valid numeric value for the length.\n\n");
                    displayHelp();
                    return 1;
                }
                break;
            case 'l':
                wantsLowCase = true;
                break;
            case 'u':
                wantsUpCase = true;
                break;
            case 'd':
                wantsDigits = true;
                break;
            case 's':
                wantsSpecials = true;
                break;
            case 'h':
                displayHelp();
                return 0;
            case 'v':
                displayVersion();
                return 0;
            case '?':
                fprintf(stderr, "Use '-h, --help' for help.\n");
                return 1;
        }
    }

    if (!wantsLowCase && !wantsUpCase && !wantsDigits && !wantsSpecials) {
        char* password = generatePassword(length, true, true, true, true);
        printf("Password: %s\n", password);
        return 0;
    }

    char* password = generatePassword(length, wantsUpCase, wantsLowCase, wantsDigits, wantsSpecials);
    printf("Password: %s\n", password);

    return 0;
}

char* generatePassword(unsigned int length, bool wantsUpCase, bool wantsLowCase, bool wantsDigits, bool wantsSpecials) {
    const char* uppercase_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char* lowercase_letters = "abcdefghijklmnopqrstuvwxyz";
    const char* digits = "0123456789";
    const char* special_chars = "!@#$%^&*()_=[{]}\\|;:'\",<.>?`~+-/*";

    // Seed the random number generator with processor time
    srand((unsigned int)clock());

    // Calculate the total number of characters based on user preferences
    unsigned int total_chars = 0;
    if (wantsUpCase) total_chars += strlen(uppercase_letters);
    if (wantsLowCase) total_chars += strlen(lowercase_letters);
    if (wantsDigits) total_chars += strlen(digits);
    if (wantsSpecials) total_chars += strlen(special_chars);

    char* password = (char*)malloc((length + 1) * sizeof(char)); // +1 for null-terminator
    if (password == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }

    for (unsigned int i = 0; i < length; i++) {
        int random_index = rand() % total_chars;

        if (wantsUpCase) {
            if (random_index < strlen(uppercase_letters)) {
                password[i] = uppercase_letters[random_index];
                continue;
            }
            random_index -= strlen(uppercase_letters);
        }
        if (wantsLowCase) {
            if (random_index < strlen(lowercase_letters)) {
                password[i] = lowercase_letters[random_index];
                continue;
            }
            random_index -= strlen(lowercase_letters);
        }
        if (wantsDigits) {
            if (random_index < strlen(digits)) {
                password[i] = digits[random_index];
                continue;
            }
            random_index -= strlen(digits);
        }
        if (wantsSpecials) {
            password[i] = special_chars[random_index];
        }
    }

    password[length] = '\0'; // Null-terminate the string
    return password;
}

// Function to display the help message
void displayHelp() {
    printf("Usage: %s [OPTIONS]\n", NAME);
    printf("Generate passwords with ease.\n\n");
    printf("Options:\n");
    printf("\t-L, --length         Specify the length of the password.\n");
    printf("\t-l, --lower-case     Enable lowercase for the password.\n");
    printf("\t-u, --upper-case     Enable uppercase for the password.\n");
    printf("\t-d, --digits         Enable digits for the password.\n");
    printf("\t-s, --special        Enable special characters for the password.\n");
    printf("\t-h, --help           Display this help message and exit.\n");
    printf("\t-v, --version        Display version and exit.\n");
}

// Function to display the version information
void displayVersion() {
    printf("%s v%.1f\n", NAME, VERSION);
}
