# Webserv

The Webserv project lead us to write our own HTTP server in C++ 98 inspired by NGINX, giving us the opportunity to understand the internal mechanisms of a web server and familiarize ourselves with HTTP requests and responses.

## Specifications

- **Language**: C++ 98 and C.
- **Features**: The server must:
  - Read a configuration file and adapt accordingly.
  - Listen on multiple ports.
  - Be non-blocking and use `select()` (or equivalent) for I/O operations.
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
git clone git@github.com:DaveInc01/web_serv.git
cd web_serv
make
./webserv path/to/config_file.conf
```

> A demo configuration file is located in `src/config/config.conf`. You can modify it to change the server behavior, listening ports, etc.

To access the site, simply enter the following line in your web browser's search bar:

```web
http://localhost:3000
```

> Port 3000 is the default port configured in the provided configuration file. If this parameter is changed, the port must be modified in the browser to access the site.

### List of Concepts Used

- Classes
  - Encapsulation
    - Visibility
    - Member attributes and functions
      - Constants
    - Accessors
  - Constructors / Destructors
    - Canonical Form of Coplien
    - Initialization list
  - Pointers to members
  - Inheritance
    - Abstract classes
- Operator Overloading
- Templates
- Containers
  - Vectors
  - Maps
- Polymorphism (Function Overloading)
  - Subtyping
- Conditional Compilation
- Exceptions
- Casting
