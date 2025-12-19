#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

class Config {
private:
    static std::unordered_map<std::string, std::string> settings;
    
public:
    static void initialize();
    static std::string get(const std::string& key, const std::string& defaultValue = "");
    static void set(const std::string& key, const std::string& value);
    static bool getBool(const std::string& key, bool defaultValue = false);
    static int getInt(const std::string& key, int defaultValue = 0);
    static float getFloat(const std::string& key, float defaultValue = 0.0f);
    
    static void loadFromFile(const std::string& filename);
    static void saveToFile(const std::string& filename);
    
    static void printAll();
};

#endif