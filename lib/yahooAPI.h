#ifndef YAHOOAPI_H
#define YAHOOAPI_H

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include "../keys/config.h"

using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

template <typename T>
void printValue(const T& value) {
    cout << "Value: " << value << endl;
}

/**
 * @function parseJsonToMap
 * @brief generates an unordered map of a json response.
 * 
 * This function is responsible for parsing a json response
 * to an unordered map for easier use.
 * 
 * @param jsonStr the json response.
 * @return unordered_map of json response.
 */
template <typename T>
auto parseJsonToMap(const T& jsonStr) -> decltype(unordered_map<string, string>()) {
    unordered_map<string, string> jsonMap;
    size_t pos = 0;
    while ((pos = jsonStr.find("\"", pos)) != string::npos) {
        size_t keyStart = pos + 1;
        size_t keyEnd = jsonStr.find("\"", keyStart);
        if (keyEnd == string::npos) break;
        string key = jsonStr.substr(keyStart, keyEnd - keyStart);

        size_t valueStart = jsonStr.find("\"", keyEnd + 1) + 1;
        size_t valueEnd = jsonStr.find("\"", valueStart);
        if (valueEnd == string::npos) break;
        string value = jsonStr.substr(valueStart, valueEnd - valueStart);

        jsonMap[key] = value;
        pos = valueEnd + 1;
    }
    return jsonMap;
}

/**
 * @function getAccessToken
 * @brief Retrieves the access token from Yahoo Fantasy API.
 * 
 * This function is responsible for obtaining the access token required
 * to authenticate and access Yahoo Fantasy API endpoints.
 * 
 * @param clientId The client ID provided by Yahoo for API access.
 * @param clientSecret The client secret provided by Yahoo for API access.
 * @param redirectUri The redirect URI registered with Yahoo for API access.
 * @param authCode The authorization code provided by Yahoo for API access.
 * @return nothing.
 */
template <typename T>
void getAccessToken(const T& clientId, const T& clientSecret, const T& redirectURI, const T& authCode) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string tokenUrl = "https://api.login.yahoo.com/oauth2/get_token?";
        cout << "Client ID: " << clientId << endl;
        std::string postFields = "client_id=" + clientId + "&client_secret=" + clientSecret + "&redirect_uri=" + redirectURI + "&code=" + authCode + "&grant_type=authorization_code";

        curl_easy_setopt(curl, CURLOPT_URL, tokenUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request and handle the response
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            cout << "Response: " << readBuffer << endl;

            // Parse the JSON response to unordered_map
            auto jsonResponse = parseJsonToMap(readBuffer);
            ACCESS_TOKEN = jsonResponse["access_token"];
            REFRESH_TOKEN = jsonResponse["refresh_token"];

            cout << "Access Token: " << ACCESS_TOKEN << endl;
            cout << "Refresh Token: " << REFRESH_TOKEN << endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }
}

#endif // YAHOOAPI_H