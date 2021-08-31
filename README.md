# peelo-xdg

![Build](https://github.com/peelonet/peelo-xdg/workflows/Build/badge.svg)

[C++17] header only alternative to [libxdg-basedir] library.

## peelo::xdg::data_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific data files.

## peelo::xdg::config_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific configuration files.

## peelo::xdg::state_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific state files.

## peelo::xdg::cache_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific non-essential data files.

## peelo::xdg::runtime_dir() -> [optional]\<[filesystem::path]\>

Directory for user-specific non-essential runtime files and other file objects
(such as sockets, named pipes, etc).

## peelo::xdg::all_data_dirs() -> [vector]\<[filesystem::path]\>

All directories for user-specific data files in users preferred order.

## xdg::all_config_dirs() -> [vector]\<[filesystem::path]\>

All directories for user-specific configuration files in users preferred order.

## xdg::home_dir() -> [optional]\<[filesystem::path]\>

Users home directory.

[C++17]: https://en.cppreference.com/w/cpp/17
[libxdg-basedir]: https://github.com/devnev/libxdg-basedir
[optional]: https://en.cppreference.com/w/cpp/utility/optional
[filesystem::path]: https://en.cppreference.com/w/cpp/filesystem/path
[vector]: https://en.cppreference.com/w/cpp/container/vector
