#include "http_server_app.h"

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_eth.h"

#include "esp_tls_crypto.h"
#include <esp_http_server.h>

static const char *TAG = "HTTP SERVER";
static httpd_req_t *REQ = NULL;
static httpd_handle_t server = NULL;

http_post_callback_t http_post_sw1_callback = NULL;
http_get_callback_t http_get_callback = NULL;
http_post_slider_callback_t http_post_slider_callback = NULL;

// ------------ IMPLEMENT EMBED IMGAGE/HTML FILE INTO ESP32 ------------------------

extern const uint8_t index_html_start[] asm("_binary_index_html_start");   // Start chuoi byte cua index
extern const uint8_t index_html_end[] asm("_binary_index_html_end");       // stop chuoi byte  cua index

/* An HTTP GET DASHBOARD handler */
static esp_err_t get_dashboard_handler(httpd_req_t *req)
{
    // Mac dinh Content-Type: text/html
    // httpd_resp_set_type(req, "image/jpg");     // Them lenh nay de Set loai du lieu truyen len web khi su dung khac loai file .html

    httpd_resp_send(req, (char *)index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}

static const httpd_uri_t getDashboardDHT11 = {
    .uri       = "/dht11Dashboard",
    .method    = HTTP_GET,
    .handler   = get_dashboard_handler,
    .user_ctx  = NULL
};

void dht11_response(char *data, int len)
{
    httpd_resp_send(REQ, (const char *)data, len);
}

/* An HTTP GET Temp and Humi handler */
static esp_err_t get_data_handler(httpd_req_t *req)
{
    REQ = req;
    http_get_callback();
    return ESP_OK;
}

static const httpd_uri_t getDataDHT11 = {
    .uri       = "/getdatadht11",
    .method    = HTTP_GET,
    .handler   = get_data_handler,
    .user_ctx  = NULL
};

/* An HTTP POST Demo handler */
static esp_err_t data_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf,req->content_len);
    printf("DATA: %s\n", buf);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t postDataDHT11 = {
    .uri       = "/data",
    .method    = HTTP_POST,
    .handler   = data_post_handler,
    .user_ctx  = NULL
};

/* An HTTP SWITCH POST handler */
static esp_err_t sw1_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf,req->content_len);
    printf("DATA: %s\n", buf);
    http_post_sw1_callback((const char *)buf);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t sw1_post_data = {
    .uri       = "/switch1",
    .method    = HTTP_POST,
    .handler   = sw1_post_handler,
    .user_ctx  = NULL
};


/* An HTTP SLIDER POST handler */
static esp_err_t slider_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf,req->content_len);
    http_post_slider_callback((const char *)buf);
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t slider_post_data = {
    .uri       = "/slider",
    .method    = HTTP_POST,
    .handler   = slider_post_handler,
    .user_ctx  = NULL
};

// --------------------- CHECK 404 ERROR ---------------------------------
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/dht11", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/dht11 URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

// ---------------------- START WEB SERVER ON ESP32 ------------------------------
void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &getDashboardDHT11);
        httpd_register_uri_handler(server, &postDataDHT11);
        httpd_register_uri_handler(server, &getDataDHT11);
        httpd_register_uri_handler(server, &sw1_post_data);
        httpd_register_uri_handler(server, &slider_post_data);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
}

// --------------------- STOP WEB SERVER ON ESP32 -----------------------
void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}

void http_post_set_callback(void *cb)
{
    if (cb != NULL)
    {
        http_post_sw1_callback = cb;
    }
}

void http_get_set_callback(void *cb)
{
    if (cb != NULL)
    {
        http_get_callback = cb;
    }
}

void http_post_set_slider_callback(void *cb)
{
    if (cb != NULL)
    {
        http_post_slider_callback = cb;
    }
}
