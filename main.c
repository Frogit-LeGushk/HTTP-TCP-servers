#include "http.h"
#include <stdio.h>


void calculator_html_page_handler(int connection, HTTPrequest * request)
{
    HTTPreply reply = {"HTTP/1.1", 200, "OK", "Content-type: text/html"};
    parse_html(connection, "pages/calculator/index.html", &reply);
}
void calculator_css_page_handler(int connection, HTTPrequest * request)
{
    HTTPreply reply = {"HTTP/1.1", 200, "OK", "Content-type: text/css"};
    parse_html(connection, "pages/calculator/style.css", &reply);
}
void calculator_js_page_handler(int connection, HTTPrequest * request)
{
    HTTPreply reply = {"HTTP/1.1", 200, "OK", "Content-type: text/javascript; charset=UTF-8"};
    parse_html(connection, "pages/calculator/js/kalk.js", &reply);
}
void calculator_mp4_page_handler(int connection, HTTPrequest * request)
{
    HTTPreply reply = {"HTTP/1.1", 200, "OK", "Content-type: video/mp4"};
    parse_html(connection, "pages/calculator/vidios/jojo.mp4", &reply);
}
void calculator_jpg_page_handler(int connection, HTTPrequest * request)
{
    HTTPreply reply = {"HTTP/1.1", 200, "OK", "Content-type: image/jpg"};
    parse_html(connection, "pages/calculator/fon/jojo.jpg", &reply);
}


int main(void)
{
    /* 1) create server */
    HTTP * http = new_server("127.0.0.1:8080");
    printf("Create server [%s]\n", "127.0.0.1:8080");

    /* 2) bind all handlers to difference paths */
    bind_http(  http,     "/index.html",      calculator_html_page_handler    );
    bind_http(  http,     "/style.css",       calculator_css_page_handler     );
    bind_http(  http,     "/js/kalk.js",      calculator_js_page_handler      );
    bind_http(  http,     "/vidios/jojo.mp4", calculator_mp4_page_handler     );
    bind_http(  http,     "/fon/jojo.jpg",    calculator_jpg_page_handler     );

    /* 3) main loop (wait requests...)*/
    listen_http(http);

    /* 4) destroy server */
    free_server(http);
    return 0;
}



