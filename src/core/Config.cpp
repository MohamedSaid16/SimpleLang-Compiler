#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::unordered_map<std::string, std::string> Config::settings = {
    {"debug", "false"},
    {"optimize", "true"},
    {"warnings", "true"},
    {"max_errors", "10"},
    {"indent_size", "4"},
    {"tab_width", "4"},
    {"encoding", "utf-8"}
};

void Config::initialize() {
    // Already initialized statically
}

std::string Config::get(const std::string& key, const std::string& defaultValue) {
    auto it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    return defaultValue;
}

void Config::set(const std::string& key, const std::string& value) {
    settings[key] = value;
}

bool Config::getBool(const std::string& key, bool defaultValue) {
    std::string value = get(key, "");
    if (value.empty()) return defaultValue;
    
    std::string lowerValue = value;
    std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);
    
    return lowerValue == "true" || lowerValue == "1" || lowerValue == "yes" || lowerValue == "on";
}

int Config::getInt(const std::string& key, int defaultValue) {
    std::string value = get(key, "");
    if (value.empty()) return defaultValue;
    
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}

float Config::getFloat(const std::string& key, float defaultValue) {
    std::string value = get(key, "");
    if (value.empty()) return defaultValue;
    
    try {
        return std::stof(value);
    } catch (...) {
        return defaultValue;
    }
}

void Config::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not load config file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;
        
        size_t equalsPos = line.find('=');
        if (equalsPos != std::string::npos) {
            std::string key = line.substr(0, equalsPos);
            std::string value = line.substr(equalsPos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            settings[key] = value;
        }
    }
}

void Config::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save config file: " << filename << std::endl;
        return;
    }
    
    file << "# SimpleLang Configuration File\n";
    file << "# Generated automatically\n\n";
    
    for (const auto& [key, value] : settings) {
        file << key << " = " << value << "\n";
    }
}

void Config::printAll() {
    std::cout << "Configuration Settings:\n";
    std::cout << "======================\n";
    
    for (const auto& [key, value] : settings) {
        std::cout << key << " = " << value << "\n";
    }
}