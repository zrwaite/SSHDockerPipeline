#include <iostream>
#include "validator.hpp"

bool validateImageName (char* imageName) {
	char * t; // first copy the pointer to not change the original
    int size = 0;
	bool hasDockerName = false;
	bool hasSlash = false;
	bool hasImageName = false;
	bool hasColon = false;

    for (t = imageName; *t != '\0'; t++) {
		size++;
		if (!hasDockerName) {
			if (isLowerCase(*t) || isNumeric(*t)) {
				hasDockerName = true;
				continue;
			}
			return false;
		}
		if (!hasSlash) {
			if (isDockerChar(*t)) continue;
			if (*t == '/') {
				hasSlash = true;
				continue;
			}
			return false;
		}
		if (!hasImageName) {
			if (isLowerCase(*t) || isNumeric(*t)) {
				hasImageName = true;
				continue;
			}
			return false;
		}
		if (!hasColon) {
			if (isDockerChar(*t)) continue;
			if (*t == ':') {
				hasColon = true;
				continue;
			}
			return false;
		}
		if (isDockerTagChar(*t)) {
			return true;
		}
		return false;
    }
    return false;
}
bool validatePort (char* port){
	char * t; // first copy the pointer to not change the original
    for (t = port; *t != '\0'; t++) {
		if (!isNumeric(*t)) return false;
	}
	return true;
}

bool isLowerCase (char c){
	return c >= 'a' && c <= 'z';
}

bool isNumeric (char c){
	return c >= '0' && c <= '9';
}

bool isUpperCase (char c) {
	return c >= 'A' && c <= 'Z';
}

bool isAlphaNumeric (char c){
	return isLowerCase(c) || isNumeric(c) || isUpperCase(c);
}

bool isDockerChar (char c) {
	return isLowerCase(c) || isNumeric(c) || c == '-' || c == '_';
}

bool isDockerTagChar(char c) {
	return isDockerChar(c) || isUpperCase(c) || isNumeric(c) || c=='.';
}

