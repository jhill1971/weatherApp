# Command-Line Current Weather App
## By James Hill 

##  Purpose:
### This program allows users to check current weather conditions in any location via the command line. Activate the program with './weather' followed by a zip code, postal code, 3-letter airport code, or city name. The program retrieves weather information for the specified location using an API.

##  Functions:
### writeMemoryCallback() 
#### The writeMemoryCallback() function serves as a callback function utilized in libcurl-based applications to handle the reception of HTTP response data. Specifically, it facilitates the storage of received data into dynamically allocated memory, enabling efficient management of incoming data streams during HTTP operations. 

### parse_weather()
#### The parse_weather() function is designed to extract relevant weather information from a JSON-formatted response obtained from a weather API. Upon receiving the JSON response, the function navigates through the structured data, retrieving essential details such as current temperature, feels-like temperature, humidity, and weather condition. By parsing and extracting these key attributes, the function prepares the data for presentation, enabling the program to display accurate and concise weather updates to the user. Additionally, error handling mechanisms are incorporated within the function to manage cases where the JSON response is invalid or lacks the expected structure, ensuring robustness and reliability in processing weather data.

### get_weather()
#### The get_weather() function orchestrates the retrieval of current weather information for a specified location by leveraging an external weather API. Upon receiving the location parameter, the function constructs an HTTP request tailored to the API's specifications using libcurl, initiating communication with the API server. Upon successful communication, the function receives a JSON-formatted response containing the requested weather data. Subsequently, it delegates the parsing of this response to the parse_weather() function to extract and interpret relevant weather details. Through this process, the get_weather() function acts as a mediator between the user's input and the external API, facilitating the seamless acquisition and presentation of up-to-date weather information directly within the program.

### main()
#### The main() function serves as the entry point and central control hub of the program, responsible for coordinating its execution flow. Upon invocation, it first verifies that the correct number of command-line arguments is provided, prompting the user with the correct usage syntax if necessary. Subsequently, it extracts the specified location parameter from the command-line arguments and passes it to the get_weather() function for weather retrieval. Finally, upon completion of the weather retrieval process, main() returns an exit status to signify the success or failure of the program's execution. Overall, the main() function ensures the proper handling of user input and initiation of the weather retrieval process, encapsulating the program's core functionality within a structured execution framework.