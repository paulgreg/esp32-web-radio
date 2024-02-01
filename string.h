
void capitalizeWords(char *str) {
    // Check if the string is not empty
    if (str != NULL) {
        // Capitalize the first letter of the string
        if (isalpha(str[0])) {
            str[0] = toupper(str[0]);
        }

        // Iterate through each character in the string
        for (int i = 1; str[i] != '\0'; i++) {
            // Capitalize the first letter of each word
            if (isalpha(str[i]) && !isalpha(str[i - 1])) {
                str[i] = toupper(str[i]);
            }
            // Convert the rest of the word to lowercase
            else if (isalpha(str[i]) && isalpha(str[i - 1])) {
                str[i] = tolower(str[i]);
            }
        }
    }
}

