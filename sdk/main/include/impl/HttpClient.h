#include <curl/curl.h>
#include <iostream>
#include <string>
#include <memory>

class HttpClient
{
public:
  /**
   * @brief Constructor for HttpClient.
   *
   * This constructor initializes the HttpClient and performs global libcurl initialization
   * using CURL_GLOBAL_DEFAULT.
   *
   * @throw std::runtime_error If libcurl initialization fails.
   */
  HttpClient();

  HttpClient(const HttpClient&) = delete;            // no need as only one httpClient could be used
  HttpClient& operator=(const HttpClient&) = delete; // no need as only one httpClient could be used

  /**
   * @brief Destructor for HttpClient.
   *
   * This destructor cleans up global libcurl resources using curl_global_cleanup.
   * It should be called when an HttpClient instance is no longer needed to release libcurl resources.
   */
  ~HttpClient();
  /**
   * @brief Fetches data from the specified URL using the provided RPC method.
   * @param url The URL to fetch data from.
   * @param rpcMethod The RPC method.
   * @return The fetched data as a string.
   */
  [[nodiscard]] const std::string invokeRPC(const std::string& url, const std::string& rpcMethod);
  /**
   * @brief This invokeREST function creates GET and POST requests.
   *  Can be further extended for supporting other HTTP
   *  methods or handle more advanced scenarios as needed.
   * @param url The URL to fetch data from.
   * @param httpMethod The HTTP method.
   * @param requestBody The HTTP request body.
   * @return The fetched data as a string.
   */
  [[nodiscard]] const std::string invokeREST(const std::string& url,
                                             const std::string& httpMethod,
                                             const std::string& requestBody);
  /**
   * @brief The callback function used for writing fetched data.
   * @param contents A pointer to the fetched data.
   * @param size The size of each element.
   * @param nmemb The number of elements.
   * @param output A pointer to the output string.
   * @return The total size of the fetched data.
   */
private:
  static size_t writeCallback(char* contents, size_t size, size_t nmemb, std::string* output);
};