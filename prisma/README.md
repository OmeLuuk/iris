# Prisma

## Overview

Prisma represents a communication bridge in the realm of distributed messaging, inspired by the natural phenomenon of light dispersion. By taking a singular message (a ray of light), Prisma disseminates it, allowing various messages (different wavelengths) to reach their respective consumers in different directions. The project encompasses a broadcast producer and numerous consumers, ensuring every message finds its path with efficiency and precision.

## Concept

Prisma utilizes the Iris server to establish a communication channel where a producer broadcasts messages, which are then consumed by multiple clients. The producer acts as the light source, while the consumers embody various paths taken by different wavelengths, each deciphering messages pertinent to their purpose.

## Architecture

- **Broadcast Producer**: The source that emits messages.
- **Broadcast Consumers**: Clients that subscribe to the producer and consume the messages.

## Features

- **Efficient Message Broadcasting**: Prisma ensures that messages from the producer are efficiently broadcasted to all connected consumers.
- **Scalability**: Designed to accommodate additional consumers with ease, ensuring scalability.
- **Dependability**: Leverages the Iris server's robustness for reliable message delivery.

## Getting Started

### Prerequisites

- A compatible C++ compiler (C++20 or later recommended)
- Ensure the Iris server is properly set up and running.