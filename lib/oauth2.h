#ifndef OAUTH2_H
#define OAUTH2_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "functions.h"
#include "../keys/config.h"

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
    std::unordered_map<std::string, std::string> postFields = {
        {"client_id", clientId},
        {"client_secret", clientSecret},
        {"redirect_uri", redirectURI},
        {"code", authCode},
        {"grant_type", "authorization_code"}
    };

    auto response = call("LOGIN", "get_token", std::forward<std::unordered_map<std::string, std::string>>(postFields));
    std::cout << "\n" << std::endl;
    std::cout << "Access Token: " << response["access_token"] << std::endl;
    std::cout << "Refresh Token: " << response["refresh_token"] << std::endl;
    std::cout << "Token Type: " << response["token_type"] << std::endl;
    std::cout << "Expires In: " << response["expires_in"] << std::endl;

    ACCESS_TOKEN = response["access_token"];
    REFRESH_TOKEN = response["refresh_token"];
    std::cout << "ACCESS_TOKEN: " << ACCESS_TOKEN << std::endl;
    std::cout << "REFRESH_TOKEN: " << REFRESH_TOKEN << std::endl;

    return;
}

#endif  // OAUTH2_H