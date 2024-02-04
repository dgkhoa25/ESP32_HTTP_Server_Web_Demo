#ifndef __HTTP_SERVER_APP_H_
#define __HTTP_SERVER_APP_H_

typedef void (*http_post_callback_t)(const char*);
typedef void (*http_post_slider_callback_t)(const char*);
typedef void (*http_get_callback_t)(void);

void start_webserver(void);
void stop_webserver(void);

void http_post_set_callback(void *cb);
void http_post_set_slider_callback(void *cb);
void http_get_set_callback(void *cb);
void dht11_response(char *data, int len);

#endif
