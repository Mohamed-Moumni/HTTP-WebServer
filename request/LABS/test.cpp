#include <iostream>
#include <unistd.h>
#include <fstream>
#include <curl/curl.h>

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string fileData;

    while (true) {
        std::ifstream file("request/LABS/file.txt", std::ios::in | std::ios::binary);
        if (file.is_open()) {
            // Read file data
            fileData.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
            file.close();
        }
        else {
            std::cout << "Failed to open file" << std::endl;
            continue;
        }

        // Initialize CURL
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            // Set POST URL
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8081/file.txt");

            // Set POST headers
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: text/plain");
            headers = curl_slist_append(headers, ("Content-Length: " + std::to_string(fileData.length())).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Set POST data
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fileData.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fileData.length());

            // Set callback for response
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Perform POST request
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
            else {
                std::cout << "Response: " << readBuffer << std::endl;
                readBuffer.clear();
            }

            // Clean up
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }
        else {
            std::cerr << "Failed to initialize CURL" << std::endl;
        }

        // Remove file
        std::remove("request/file.txt");
        sleep(1);

        // Sleep for 1 second
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}