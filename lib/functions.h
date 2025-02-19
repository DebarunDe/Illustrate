#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @function WriteCallback
 * @brief callback function for curl write.
 * 
 * This function is responsible for handling the curl write callback.
 * 
 * @param contents the contents to write.
 * @param size the size of the contents.
 * @param nmemb the number of members.
 * @param userp the user pointer.
 * @return size_t the size of the contents.
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
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
auto parseJsonToMap(const T& jsonStr) 
-> decltype(std::unordered_map<std::string, std::string>()) {
    std::unordered_map<std::string, std::string> jsonMap;
    size_t pos = 0;
    while ((pos = jsonStr.find("\"", pos)) != std::string::npos) {
        size_t keyStart = pos + 1;
        size_t keyEnd = jsonStr.find("\"", keyStart);
        if (keyEnd == std::string::npos) break;
        std::string key = jsonStr.substr(keyStart, keyEnd - keyStart);

        size_t valueStart = jsonStr.find("\"", keyEnd + 1) + 1;
        size_t valueEnd = jsonStr.find("\"", valueStart);
        if (valueEnd == std::string::npos) break;
        std::string value = jsonStr.substr(valueStart, valueEnd - valueStart);

        jsonMap[key] = value;
        pos = valueEnd + 1;
    }
    return jsonMap;
}

/**
 * @function parseMapToPostString
 * @brief generates a post string from an unordered map.
 * 
 * This function is responsible for parsing an unordered map
 * to a post string for use in curl requests.
 * 
 * @param postFields the post fields to send with the request.
 * @return std::string the post string.
 */
template <typename T>
auto parseMapToPostString(const T& postFields) 
-> decltype(std::string()) {
    std::string postFieldsStr;
    for (auto& pair : postFields) {
        postFieldsStr += pair.first + "=" + pair.second + "&";
    }
    // remove trailing '&' from postFieldsStr
    postFieldsStr.pop_back();
    return postFieldsStr;
}

/**
 * @function call
 * @brief performs a curl request to a given URL.
 * 
 * This function is responsible for performing a curl request
 * to a given URL with the provided postFields.
 * 
 * @param function the type of function to call.
 * @param command the command to execute.
 * @param postFields the post fields to send with the request.
 * @return unordered_map of json response.
 */
template <typename F, typename C, typename P>
auto call(const F& function, const C& command, const P& postFields) 
-> decltype(std::unordered_map<std::string, std::string>()) {
    // generate URL based on function type:
    // LOGIN: https://api.login.yahoo.com/oauth2/
    // FANTASY: https://fantasysports.yahooapis.com/fantasy/v2/
    std::string url;
    auto jsonResponse = std::unordered_map<std::string, std::string>();
    if (function == "LOGIN") {
        url = "https://api.login.yahoo.com/oauth2/";
    } else if (function == "FANTASY") {
        url = "https://fantasysports.yahooapis.com/fantasy/v2/";
    } else {
        std::cerr << "Invalid function type." << std::endl;
        return jsonResponse;
    }

    // initialize curl
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    url += command;
    url += "?";

    // generate postFields string
    std::string postFieldsStr = parseMapToPostString(postFields);

    // perform curl request
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFieldsStr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request and handle the response
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            if (readBuffer.empty()) {
                std::cerr << "Empty response." << std::endl;
            }

            // Parse the JSON response to unordered_map
            jsonResponse = parseJsonToMap(readBuffer);
        }
    } else {
        std::cerr << "Curl failed to initialize." << std::endl;
    }

    // Clean up
    curl_easy_cleanup(curl);
    return jsonResponse;
}

#endif // FUNCTIONS_H