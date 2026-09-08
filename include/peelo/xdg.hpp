/*
 * Copyright (c) 2021-2025, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#if !defined(_WIN32)
# include <pwd.h>
# include <unistd.h>
#endif

namespace peelo::xdg
{
  namespace internal
  {
#if defined(_WIN32)
    static constexpr char path_separator = ';';
#else
    static constexpr char path_separator = ':';
#endif

    inline std::optional<std::string>
    getenv_string(const char* name)
    {
#if defined(_WIN32) && defined(_MSC_VER)
      char* value = nullptr;
      std::size_t length = 0;

      if (::_dupenv_s(&value, &length, name) != 0)
      {
        return std::nullopt;
      }

      if (!value || !*value)
      {
        std::free(value);

        return std::nullopt;
      }

      std::string result{value};

      std::free(value);

      return result;
#else
      const auto value = std::getenv(name);

      if (!value || !*value)
      {
        return std::nullopt;
      }

      return std::string{value};
#endif
    }

    template<class T = std::filesystem::path>
    std::vector<T>
    get_multiple_dirs(const char* env_variable_name)
    {
      std::vector<T> result;
      const auto env_value = getenv_string(env_variable_name);

      if (env_value)
      {
        auto begin = env_value->c_str();
        auto end = begin;

        for (; *end; ++end)
        {
          if (*end != path_separator)
          {
            continue;
          }
          else if (end - begin > 0)
          {
            result.emplace_back(std::string(begin, end - begin));
          }
          begin = end + 1;
        }
        if (end - begin > 0)
        {
          result.emplace_back(std::string(begin, end - begin));
        }
      }

      return result;
    }
  }

  /**
   * Returns path to users home directory, if it can be determined.
   */
  template<class T = std::filesystem::path>
  std::optional<T>
  home_dir()
  {
    if (const auto home = internal::getenv_string("HOME"))
    {
      return *home;
    }

#if defined(_WIN32)
    if (const auto userprofile = internal::getenv_string("USERPROFILE"))
    {
      return *userprofile;
    }

    const auto homedrive = internal::getenv_string("HOMEDRIVE");
    const auto homepath = internal::getenv_string("HOMEPATH");

    if (homedrive && homepath)
    {
      return T(*homedrive) / *homepath;
    }
#else
    const auto pw = ::getpwuid(::getuid());

    if (pw)
    {
      return pw->pw_dir;
    }
#endif

    return std::nullopt;
  }

  /**
   * Returns path to XDG data directory, if it can be determined.
   */
  template<class T = std::filesystem::path>
  std::optional<T>
  data_dir()
  {
    if (const auto env_variable = internal::getenv_string("XDG_DATA_HOME"))
    {
      return *env_variable;
    }
    else if (const auto home = home_dir())
    {
      return *home / ".local" / "share";
    }

    return std::nullopt;
  }

  /**
   * Returns all XDG data directories in users preferred order, or empty vector
   * if they cannot be determined.
   */
  template<class T = std::filesystem::path>
  std::vector<T>
  all_data_dirs()
  {
    auto result = internal::get_multiple_dirs("XDG_DATA_DIRS");

#if !defined(_WIN32)
    if (result.empty())
    {
      result.emplace_back("/usr/local/share");
      result.emplace_back("/usr/share");
    }
#endif

    return result;
  }

  /**
   * Returns path to XDG configuration directory, if it can be determined.
   */
  template<class T = std::filesystem::path>
  std::optional<T>
  config_dir()
  {
    if (const auto env_variable = internal::getenv_string("XDG_CONFIG_HOME"))
    {
      return *env_variable;
    }
    else if (const auto home = home_dir())
    {
      return *home / ".config";
    }

    return std::nullopt;
  }

  /**
   * Returns all XDG configuration directories in users preferred order, or
   * empty vector if they cannot be determined.
   */
  template<class T = std::filesystem::path>
  std::vector<T>
  all_config_dirs()
  {
    auto result = internal::get_multiple_dirs("XDG_CONFIG_DIRS");

#if !defined(_WIN32)
    if (result.empty())
    {
      result.emplace_back("/etc/xdg");
    }
#endif

    return result;
  }

  /**
   * Returns path to XDG state directory, if it can be determined.
   */
  template<class T = std::filesystem::path>
  std::optional<T>
  state_dir()
  {
    if (const auto env_variable = internal::getenv_string("XDG_STATE_HOME"))
    {
      return *env_variable;
    }
    else if (const auto home = home_dir())
    {
      return *home / ".local" / "state";
    }

    return std::nullopt;
  }

  /**
   * Returns path to XDG cache directory, if it can be determined.
   */
  template<class T = std::filesystem::path>
  std::optional<T>
  cache_dir()
  {
    if (const auto env_variable = internal::getenv_string("XDG_CACHE_HOME"))
    {
      return *env_variable;
    }
    else if (const auto home = home_dir())
    {
      return *home / ".cache";
    }

    return std::nullopt;
  }

  /**
   * Returns path to XDG runtime directory, if it can be determined.
   */
  template<class T = std::filesystem::path>
  std::optional<T>
  runtime_dir()
  {
    if (const auto env_variable = internal::getenv_string("XDG_RUNTIME_DIR"))
    {
      return *env_variable;
    }

    return std::nullopt;
  }
}
