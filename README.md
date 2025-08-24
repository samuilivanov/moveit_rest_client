MoveIt REST Client
# MoveIt REST Client (CLI & Server)

**Description**  
This project is a C++ application intended to include both a **command-line client** and a **server component** for interacting with a MoveIt REST API. Packaged in a Docker container for easy cross-platform usage.

> **Note:** The server component is not implemented yet; currently only the CLI client is functional.

## Features

- Authenticate with username and password (password is masked).  
- Upload files to the authenticated user’s home folder.
- Fully containerized with Docker

## Requirements
**Host Requirements**
- Docker ≥ 27.0
- Needed to build and run the containerized client.

**Optional (for local build / testing)**
- C++ compiler supporting C++17 (e.g., g++ >= 11 or clang >= 14)
- CMake ≥ 3.20
- make
- libcurl development headers (for local build if not using Docker)
- doctest (for running unit tests)

## Building the Docker Image

From the project root:

```bash
docker build -t moveit-client .
docker run --rm -it \
  -v /path/to/files:/moveit_rest_client/input \
  moveit-client --mode cli --user ${user} --url ${base_url} --file /moveit_rest_client/input/${file}$
```
    -it ensures the container attaches a TTY so you can enter your password.
    -v /host/path:/container/path mounts host files into the container.

## Building the locally

```bash
mkdir build && cd build
cmake ..
make
./app/app --help
