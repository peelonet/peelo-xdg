# peelo-xdg

![Build](https://github.com/peelonet/peelo-xdg/workflows/Build/badge.svg)

[C++17] header only alternative to [libxdg-basedir] library.

[Doxygen generated API documentation](https://peelonet.github.io/peelo-xdg/)

## API

### peelo::xdg::data_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific data files.

### peelo::xdg::config_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific configuration files.

### peelo::xdg::state_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific state files.

### peelo::xdg::cache_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific non-essential data files.

### peelo::xdg::runtime_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific non-essential runtime files and other file objects
(such as sockets, named pipes, etc).

### peelo::xdg::all_data_dirs() -> [vector]\<[filesystem::path]\>

All directories for user-specific data files in users preferred order.

### peelo::xdg::all_config_dirs() -> [vector]\<[filesystem::path]\>

All directories for user-specific configuration files in users preferred order.

### peelo::xdg::home_dir() -> [optional]\<[filesystem::path]\>

Users home directory.

## Example

Lets first create an `CMakeLists.txt` file for our project:

```cmake
cmake_minimum_required(VERSION 3.12)
project(MyProject LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  PeeloXdg
  GIT_REPOSITORY
    https://github.com/peelonet/peelo-xdg.git
  GIT_TAG
    1.0.0
)
FetchContent_MakeAvailable(PeeloXdg)

add_executable(MyProject main.cpp)
target_link_libraries(MyProject PRIVATE PeeloXdg)
```

Then `main.cpp` like this:

```c++
#include <iostream>

#include <peelo/xdg.hpp>

int main(int argc, char** argv)
{
  if (const auto config_dir = peelo::xdg::config_dir())
  {
    const auto app_config_dir = *config_dir / "MyProject";

    // Create application config directory, if it does not already exist.
    if (!std::filesystem::is_directory(app_config_dir))
    {
      std::filesystem::create_directories(app_config_dir);
      std::cout << "Configuration directory created." << std::endl;
    } else {
      std::cout << "Configuration directory already exists." << std::endl;
    }
  } else {
    std::cout << "Could not determine configuration directory." << std::endl;
  }

  return 0;
}
```

After compiling the application and running it, an directory called `MyProject`
will be created under the XDG configuration directory, if one can be
determined based on the environment variable `XDG_CONFIG_DIR`.

[C++17]: https://en.cppreference.com/w/cpp/17
[libxdg-basedir]: https://github.com/devnev/libxdg-basedir
[optional]: https://en.cppreference.com/w/cpp/utility/optional
[filesystem::path]: https://en.cppreference.com/w/cpp/filesystem/path
[vector]: https://en.cppreference.com/w/cpp/container/vector
