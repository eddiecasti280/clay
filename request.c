#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Structure to hold response data
struct ResponseData {
    char* data;
    size_t size;
};

// Callback function to handle response
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct ResponseData* mem = (struct ResponseData*)userp;
    
    char* ptr = realloc(mem->data, mem->size + realsize + 1);
    if(!ptr) {
        printf("Error: out of memory!\n");
        return 0;
    }
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

// Function to make GET request
void makeGetRequest(const char* url) {
    CURL* curl;
    CURLcode res;
    struct ResponseData response = {0};
    
    // Initialize response struct
    response.data = malloc(1);
    response.size = 0;
    
    curl = curl_easy_init();
    if(curl) {
        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        // Set callback function to handle response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        
        // Perform request
        res = curl_easy_perform(curl);
        
        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                    curl_easy_strerror(res));
        } else {
            printf("Response:\n%s\n", response.data);
        }
        
        // Cleanup
        curl_easy_cleanup(curl);
    }
    
    free(response.data);
}

// Function to make POST request
void makePostRequest(const char* url, const char* data) {
    CURL* curl;
    CURLcode res;
    struct ResponseData response = {0};
    struct curl_slist* headers = NULL;
    
    // Initialize response struct
    response.data = malloc(1);
    response.size = 0;
    
    curl = curl_easy_init();
    if(curl) {
        // Set headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        // Set URL and POST data
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        // Set callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
        
        // Perform request
        res = curl_easy_perform(curl);
        
        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                    curl_easy_strerror(res));
        } else {
            printf("Response:\n%s\n", response.data);
        }
        
        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    free(response.data);
}

int main() {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Example GET request
    printf("Making GET request...\n");
    makeGetRequest("https://api.example.com/data");
    
    // Example POST request
    printf("\nMaking POST request...\n");
    const char* postData = "{\"key\": \"value\"}";
    makePostRequest("https://api.example.com/post", postData);
    
    // Cleanup global libcurl resources
    curl_global_cleanup();
    
    return 0;
}