# Webserv

The Webserv project lead us to write our own HTTP server in C++ 98 inspired by NGINX, giving us the opportunity to understand the internal mechanisms of a web server and familiarize ourselves with HTTP requests and responses.

## Specifications

- **Language**: C++ 98 and C.
- **Features**: The server must:
  - Read a configuration file and adapt accordingly.
  - Listen on multiple ports.
  - Be non-blocking and use `poll()` (or equivalent) for I/O operations.
  - Support GET, POST, and DELETE methods.
  - Serve a static website.
  - Handle file uploads from clients.
  - Support CGI for certain file extensions.
  - Be robust and never crash unexpectedly.
  - Comply with HTTP 1.1, using NGINX as a reference for response behaviors and headers.
- No external libraries (including the Boost set) are allowed.
- The use of `fork` is restricted to CGI purposes only.

## Usage

```shell
git clone git@github.com:semx2a/webserv.git
cd webserv/server
make
./webserv path/to/config_file.conf
```

> A demo configuration file is located in `neoserv/conf/testwebsite.conf`. You can modify it to change the server behavior, listening ports, etc.

To access the site, simply enter the following line in your web browser's search bar:

```web
http://localhost:8080
```

> Port 8080 is the default port configured in the provided configuration file. If this parameter is changed, the port must be modified in the browser to access the site.

## Project Learnings

- In-depth understanding of the HTTP protocol and its methods.
- Mastery of network programming in C and C++, especially managing non-blocking connections and I/O multiplexing. We chose `epoll` as the multiplexer for the following reasons:
  - Better Performance: `epoll` operates on an event-driven model, where the kernel informs the application about file descriptors ready for I/O operations. This avoids unnecessary system calls (unlike `select` or `poll` which iterate over the entire set of files) and reduces the complexity from O(n) to O(1) in many scenarios.
  - Flexibility and Control: `epoll` offers precise control over the types of events monitored for each file descriptor without having to remove and re-add it, which is not possible with `select` or `poll`.
- Managing a complex project in C++, adhering to coding standards, and producing clean and well-organized code.
- Ability to create and read configuration files to customize server behavior.
- Understanding how web servers handle requests, serve static content, manage file uploads, and execute CGI.
- Developing debugging and testing skills to ensure the server is reliable and performs well even under load.
