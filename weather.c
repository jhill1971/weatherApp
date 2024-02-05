#include <curl/curl.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define API_KEY "25f652faa0584795b78205001243001"
#define BASE_URL "http://api.weatherapi.com/v1/current.json?key=%s&q=%s&aqi=no"

/*
The MemoryStruct struct is a data structure used to store dynamically allocated 
memory for receiving and holding HTTP response data during libcurl operations. 
It typically includes a pointer to the allocated memory block (memory) and a size 
variable (size) to track the current size of the allocated memory.
*/
struct MemoryStruct {
    char *memory;
    size_t size;
};

/*
The WriteMemoryCallback() function is a callback function used in conjunction with
 libcurl's easy interface to store received data into memory, typically used for handling
  HTTP responses during data transfer operations.
*/
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        fprintf(stderr, "not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
/*
The parse_weather() function parses a JSON response containing current weather data,
extracting relevant information such as temperature, feels-like temperature, humidity, 
and condition, and outputs formatted weather details.
*/
void parse_weather(const char *json_response) {
    struct json_object *root, *location_obj, *current_obj, *temp_c_obj, *feelslike_c_obj, *humidity_obj, *condition_obj;
    double temp_c, feelslike_c;
    int humidity;
    const char *condition;

    root = json_tokener_parse(json_response);

    if (root != NULL && json_object_object_get_ex(root, "current", &current_obj)) {
        // Extract location information
        json_object_object_get_ex(root, "location", &location_obj);
        const char *name = json_object_get_string(json_object_object_get(location_obj, "name"));
        const char *region = json_object_get_string(json_object_object_get(location_obj, "region"));
        const char *country = json_object_get_string(json_object_object_get(location_obj, "country"));
        printf("Location: %s, %s, %s\n", name, region, country);

        // Extract current weather data
        json_object_object_get_ex(current_obj, "temp_c", &temp_c_obj);
        json_object_object_get_ex(current_obj, "feelslike_c", &feelslike_c_obj);
        json_object_object_get_ex(current_obj, "humidity", &humidity_obj);
        json_object_object_get_ex(current_obj, "condition", &condition_obj);

        // Convert json_object pointers to appropriate types
        temp_c = json_object_get_double(temp_c_obj);
        feelslike_c = json_object_get_double(feelslike_c_obj);
        humidity = json_object_get_int(humidity_obj);
        condition = json_object_get_string(json_object_object_get(condition_obj, "text"));

        // Output formatted weather information
        printf("Current Temperature: %.1f°C\n", temp_c);
        printf("Feels Like: %.1f°C\n", feelslike_c);
        printf("Humidity: %d%%\n", humidity);
        printf("Condition: %s\n", condition);
    } else {
        fprintf(stderr, "Error: Unable to parse JSON response.\n");
    }

    // Release JSON object
    json_object_put(root);
}

/*
The get_weather() function retrieves current weather data from an external API 
based on the provided location information, handling the HTTP request using libcurl 
and parsing the JSON response to extract weather details using the parse_weather() function.
*/
void get_weather(const char *location) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    char url[256];
    snprintf(url, sizeof(url), BASE_URL, API_KEY, location);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        // Print JSON response
        // printf("JSON Response:\n%s\n", chunk.memory);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            // Parse JSON response
            parse_weather(chunk.memory);
        }

        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    curl_global_cleanup();
}

/* The main() function gets the location information from the command line arguments.
 User input can be a zip code, postal code, city name, or 3-letter airport code
 This information is passed to the get_weather() function. If the command line argument is 
 incorrect, the user is prompted with the correct input format.
 */
int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <location>\n", argv[0]);
        return 1;
    }

    char *location = argv[1];
    get_weather(location);

    return 0;
}