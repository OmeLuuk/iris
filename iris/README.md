# Iris Server

## Overview

Iris Server is a low latency messaging server crafted with a dedication to minimalism and performance. Developed to manage message transmission with precision and ease, it serves as a foundational element in the communication ecosystem. It is designed to handle and forward messages.

## Features
- **Direct Response**: Iris runs on a ```epoll``` loop and always either waits for incoming messages or handles messages it received.
- **Broadcasting over TCP**: Iris facilitates Prisma, receiving broadcast producer's messages and forwarding them reliably to all consumers over TCP streams. __(WIP)__
- **DNS Server**: Iris has a static address and port, making it the only application in the ecosystem that is guaranteed to be discoverable. As such, it knows of all other applications that are connected and can provide address info to applications so they can connect to each other directly. __(WIP)__

## Features

- **Customizable Logging**: Detailed logging with distinct levels (INFO, ERROR, DEBUG) to facilitate efficient debugging and monitoring, with compile-time log level management.
- **Monorepo Architecture**: An encapsulated environment with common code extracted to a universal directory, simplifying dependency management and promoting modularity.
- **Enhanced Message Handling**: Efficient and precise message processing and distribution with low overhead, ensuring optimal performance and resource utilization.
- **Scalable**: Designed with future expansions and adaptability in mind, Iris is poised for future enhancements and integrations.

## Getting Started
Iris requires no third party libraries and can be built on any relevant Linux system. It is being developed on Ubuntu 22.04.

### Prerequisites

- A compatible C++ compiler (C++20 or later recommended)
- A compatible Linux distribution

### Usage
Most of the code that has been developed specifically for Iris, such as message types and connection managers can be reused in other applications and is therefore extracted out into the common folder. Please find supported messaging types in ```types.h```.

## Developing with Iris

### Monorepo and Dependency Management

Iris employs a monorepo architecture to contain common code within a unified directory, mitigating dependency management complexities. All components, from the IrisServer to the IrisClient, and potential future elements, are encapsulated within the same repository.
