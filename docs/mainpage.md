# Plotly.cpp API Documentation

![Build Status](https://img.shields.io/github/actions/workflow/status/yhisaki/plotly.cpp/ci.yml?branch=main)
[![codecov](https://codecov.io/gh/yhisaki/plotly.cpp/graph/badge.svg?token=NsLJgwCpau)](https://codecov.io/gh/yhisaki/plotly.cpp)
![Version](https://img.shields.io/github/v/tag/yhisaki/plotly.cpp?label=version)
![License](https://img.shields.io/github/license/yhisaki/plotly.cpp)
![C++ Standard](https://img.shields.io/badge/C%2B%2B-17%2B-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

## Overview

**Plotly.cpp** brings the power of [Plotly.js](https://plotly.com/javascript/) to C++. This library provides a modern C++17 interface for creating interactive data visualizations with real-time updates, event handling, and export capabilities.

![Plotly.cpp Demo](/docs/images/gallery.png)

## Key Features

- 🔗 **Plotly.js API Mapping** - Translation of most Plotly.js methods

![Plotly.js API Mapping](/docs/images/plotly_cpp_api.svg)

- 🎨 **Advanced Visualizations** - Rich variety of plot types. See [gallery](gallery/README.md) for more examples.

![Advanced Visualizations](/docs/images/financial_candlestick.png)

- ⚡ **Real-Time Updates** - Stream data with smooth animations and live updates

![Real-Time Updates](/docs/images/double_pendulum.gif)

- 🔄 **Bidirectional Events** - Handle user interactions from C++

![Bidirectional Events](/docs/images/event_handling.gif)

## Installation & Quick Start

### Prerequisites

- **Ubuntu Linux** (tested platform)
- **Chrome/Chromium** browser
- **C++17 or higher**

### Installation

#### Install from deb package (Recommended)

```bash
wget https://github.com/yhisaki/plotly.cpp/releases/download/v0.1.0/libplotly-cpp-0.1.0-Linux.deb
sudo apt install ./libplotly-cpp-0.1.0-Linux.deb
```

#### Install from FetchContent

Add to your CMake project using FetchContent:

```cmake
include(FetchContent)

FetchContent_Declare(
    plotly-cpp
    GIT_REPOSITORY https://github.com/yhisaki/plotly.cpp.git
    GIT_TAG v0.1.0
)

FetchContent_MakeAvailable(plotly-cpp)
```

#### Usage

After installation, add the following to your `CMakeLists.txt`:

```cmake
find_package(plotly-cpp REQUIRED)

target_link_libraries(your_target plotly-cpp::plotly-cpp)
```

#### Dependencies

Plotly.cpp requires the following dependencies:

- [**nlohmann/json**](https://github.com/nlohmann/json) - JSON serialization/deserialization. You can install it by `sudo apt install nlohmann-json3-dev`.

## Architecture

The library uses a **client-server architecture**:

1. **C++ Backend** - Your application using `plotly::Figure`
   - WebSocket server for real-time communication
   - HTTP server serving the web frontend
   - JSON-RPC protocol for structured messaging

2. **JavaScript Frontend** - Browser-based rendering engine
   - Plotly.js runtime for visualization
   - Event bridge for user interactions

You can find more details in [Architecture Overview](architecture.md).

## Quick Start

```cpp
#include "plotly/plotly.hpp"

int main() {
    plotly::Figure fig;
    fig.openBrowser();

    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {1, 4, 2, 8, 5};

    plotly::Object trace = {
        {"x", x}, {"y", y},
        {"type", "scatter"},
        {"mode", "lines+markers"}
    };

    plotly::Object layout = {{"title", {{"text", "Hello World!"}}}};

    fig.newPlot(plotly::Array{trace}, layout);
    fig.waitClose();
    return 0;
}
```

![Hello World](/docs/images/hello-world.png)

## Core Classes

| Class | Description |
|-------|-------------|
| @ref plotly::Figure | Main plotting interface - your primary entry point |
| @ref plotly::Object | JSON-compatible data container (`nlohmann::json` alias) |

## Essential Methods

| Method | JavaScript Equivalent | Purpose |
|--------|----------------------|---------|
| @ref plotly::Figure::newPlot() | [`Plotly.newPlot()`](https://plotly.com/javascript/plotlyjs-function-reference/#plotlynewplot) | Create new plot |
| @ref plotly::Figure::update() | [`Plotly.update()`](https://plotly.com/javascript/plotlyjs-function-reference/#plotlyupdate) | Update data & layout |
| @ref plotly::Figure::extendTraces() | [`Plotly.extendTraces()`](https://plotly.com/javascript/plotlyjs-function-reference/#plotlyextendtraces) | Stream real-time data |
| @ref plotly::Figure::on() | [Event listeners](https://plotly.com/javascript/plotlyjs-events/) | Handle user interactions |

---

For complete examples and advanced usage, visit the [project repository](https://github.com/yhisaki/plotly.cpp).
