# webserv

> A 42 School project — building an HTTP/1.1 web server from scratch in C++98.

---

## About

`webserv` is a fully functional HTTP web server written in C++, inspired by **NGINX**. The goal is to understand how web servers work at a low level — socket management, HTTP request parsing, response building, CGI execution, and configuration file parsing.

The server handles multiple clients simultaneously using non-blocking I/O, and is configured via a custom `.cf` configuration file.

---

## Features

- **HTTP/1.1** compliant
- **Non-blocking I/O** — handles multiple simultaneous connections
- **Custom config file** parsing (`.cf` format, inspired by NGINX)
- **Multiple virtual servers** on different ports/hosts
- **HTTP methods:** `GET`, `POST`, `DELETE`
- **Static file serving** from a configurable root directory
- **Custom error pages** (400, 403, 404, 405, 500...)
- **Redirections** (301/302)
- **Directory listing** (autoindex)
- **File upload** support
- **Chunked transfer encoding**
- Custom exception handling

---

## Project Structure

```
.
├── Webserv/            # Core server source files (socket, server, client, response)
├── Http_protocol/      # HTTP request & response parsing
├── root/               # Default web root (HTML, CSS, SCSS, JS assets)
├── config.cf           # Default server configuration file
├── parse_config.cpp    # Configuration file parser
├── get_value.cpp       # Config value extraction helpers
├── Exception.cpp/.hpp  # Custom exception handling
├── main.cpp / main.hpp # Entry point
└── Makefile
```

---

## Configuration

The server is configured via `config.cf`. Example block:

```
server{
	server_name 	 	nom;
	listen 				8080;
	root ./root/server1;
	default	index.html;
	protocol	GET;
	host	0.0.0.0;
	body_size 1000000;
	{
		location /;
		methods GET POST;
		root ./root/server1;
		cgi on;
		default index.html;
	}
	{
		location /2/;
		methods GET POST;
		root ./root/server2;
		default index.html;
		cgi on;
	}
}

server{
	server_name 	 	deuxieme;
	listen 				8081;
	root ./root/server2;
	default	index.html;
	protocol	GET POST;
	host	0.0.0.0;
	{
		location /;
		methods GET POST;
		root ./root/server2;
		autoindex on;
		upload_path  ./root/server1/upload;
	}
	{
		location /cgi/;
		methods GET POST;
		root ./root/server1;
		autoindex on;
		cgi on;
	}
}
```

Key directives:

| Directive      | Description                              |
|----------------|------------------------------------------|
| `listen`       | Port to listen on                        |
| `server_name`  | Virtual host name                        |
| `root`         | Path to the web root directory           |
| `default`      | Default index file                       |
| `methods`      | Allowed HTTP methods for a location      |
| `autoindex`    | Enable/disable directory listing         |
| `upload_path`  | Directory for uploaded files             |
| `cgi`          | CGI script interpreter                   |

---

## Usage

### Build

```bash
make
```

### Run

```bash
./webserv config.cf
```

Then open your browser at `http://localhost:8080`.

### Run with a custom config

```bash
./webserv path/to/your.cf
```

---

## Requirements

- **OS:** Linux or macOS
- **Compiler:** `c++` with `-std=c++98`
- No external libraries — standard POSIX/socket APIs only

---

## HTTP Methods Supported

| Method   | Description                        |
|----------|------------------------------------|
| `GET`    | Retrieve a resource                |
| `POST`   | Submit data / upload files         |
| `DELETE` | Delete a resource                  |

---

## Author

**CarsonJo** — [github.com/CarsonJo](https://github.com/CarsonJo)
