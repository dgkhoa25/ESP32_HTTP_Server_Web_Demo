set(app_src http_server_app.c)
set(pri_req esp_http_server nvs_flash esp-tls)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})

