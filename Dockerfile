# Stage 1: Build
FROM alpine:3.22.1 AS build

# Install build dependencies
RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    git \
    curl \
    curl-dev \
    openssl-dev \
    zlib-dev

# Get doctest header
RUN mkdir -p /usr/local/include/doctest && \
    curl -L https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h \
    -o /usr/local/include/doctest/doctest.h

WORKDIR /moveit_rest_client

# Copy project
COPY . /moveit_rest_client

# Create build directory and compile
RUN mkdir -p build && cd build && cmake .. && make -j$(nproc)

# Stage 2: Runtime
FROM alpine:3.22.1

# Install runtime dependencies
RUN apk add --no-cache libstdc++ libgcc libcurl openssl zlib curl

WORKDIR /moveit_rest_client

# Copy binary from build stage
COPY --from=build /moveit_rest_client/build/app/app .

# Set entrypoint
ENTRYPOINT ["./app"]
CMD []
