#include <iostream>
// gameserver side of this matchmaker made by KERA!
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>

#if defined(_WIN32)
#include <windows.h>
#endif

const std::string MATCHMAKER_URL = "http://localhost:7777";
const std::string SERVER_NAME = "yes";
const std::string SERVER_REGION = "us";
const std::string PLAYLIST = "deathmatch";

std::string generateRandomKey(int length = 32) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::ostringstream ss;
    for (int i = 0; i < length; i++) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string generateRandomID(int length = 16) {
    return generateRandomKey(length);
}

bool postRequest(const std::string &url) {
    CURL *curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}

int main() {
    std::string serverKey = generateRandomKey();
    std::string serverId = generateRandomID();

    std::cout << "Server Key: " << serverKey << std::endl;
    std::cout << "Server ID: " << serverId << std::endl;
    std::string startUrl = MATCHMAKER_URL + "/start?server=" + SERVER_NAME +
                           "&privateKey=" + serverKey +
                           "&serverId=" + serverId +
                           "&region=" + SERVER_REGION +
                           "&playlist=" + PLAYLIST;

    std::cout << "Starting server..." << std::endl;
    if (postRequest(startUrl)) {
        std::cout << "Server started successfully." << std::endl;
    } else {
        std::cout << "Failed to start server." << std::endl;
        return 1;
    }
    std::cin.get();
  
    std::string closeUrl = MATCHMAKER_URL + "/close?server=" + SERVER_NAME +
                           "&privateKey=" + serverKey +
                           "&serverId=" + serverId;

    std::cout << "Closing server..." << std::endl;
    if (postRequest(closeUrl)) {
        std::cout << "Server closed successfully." << std::endl;
    } else {
        std::cout << "Failed to close server." << std::endl;
    }

    return 0;
}
