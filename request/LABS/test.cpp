#include <iostream>
#include <unistd.h>
#include <curl/curl.h>

// Callback function to write response data
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::cout << "Server response: " << std::string(static_cast<char*>(contents), size * nmemb) << std::endl;
    return size * nmemb;
}

int main() {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return 1;
    }

    // Set the POST URL and data
    const std::string url = "http://10.12.6.1:8081/file.txt";
    const std::string postData = "Hello, worldgkljdlfjgsdjfglksdjfgjdklsfgjkjsdgfljdsfgjl;gsl;djsfglds!"; // Change this to the desired POST data

    // Set the request options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    
    // Set the "Content-Type" header to "text/plain"
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: text/plain");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Enter infinite loop to send POST requests
    while (true) {
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to send POST request: " << curl_easy_strerror(res) << std::endl;
        }

    }

    // Cleanup and exit
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
