/*
 * Copyright (c) 2021, peelo.net
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
#include <vector>

#if !defined(_WIN32)
# include <pwd.h>
# include <unistd.h>
#endif

namespace peelo::xdg
{
  namespace internal
  {
    inline std::vector<std::filesystem::path>
    get_multiple_dirs(const char* env_variable_name)
    {
#if defined(_WIN32)
      static const char separator = ';';
#else
      static const char separator = ':';
#endif
      std::vector<std::filesystem::path> result;

      if (const auto env_variable = std::getenv(env_variable_name))
      {
        const std::string s = env_variable;
        std::string::size_type start = 0;
        std::string::size_type end = s.find(separator);

        while (end != std::string::npos)
        {
          result.push_back(s.substr(start, end - start));
          start = end + 1;
          end = s.find(separator, start);
        }
        if (end - start > 0)
        {
          result.push_back(s.substr(start, end - start));
        }
      }

      return result;
    }
  }

  /**
   * Returns path to users home directory, if it can be determined.
   */
  inline std::optional<std::filesystem::path>
  home_dir()
  {
    const auto home = std::getenv("HOME");

    if (!home)
    {
#if defined(_WIN32)
      const auto userprofile = std::getenv("USERPROFILE");

      if (!userprofile)
      {
        const auto homedrive = std::getenv("HOMEDRIVE");
        const auto homepath = std::getenv("HOMEPATH");

        if (homedrive && *homedrive && homepath && *homepath)
        {
          return std::filesystem::path(homedrive) / homepath;
        }
      } else {
        return userprofile;
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

    return home;
  }

  /**
   * Returns path to XDG data directory, if it can be determined.
   */
  inline std::optional<std::filesystem::path>
  data_dir()
  {
    if (const auto env_variable = std::getenv("XDG_DATA_HOME"))
    {
      return env_variable;
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
  inline std::vector<std::filesystem::path>
  all_data_dirs()
  {
    auto result = internal::get_multiple_dirs("XDG_DATA_DIRS");

#if !defined(_WIN32)
    if (result.empty())
    {
      result.push_back("/usr/local/share");
      result.push_back("/usr/share");
    }
#endif

    return result;
  }

  /**
   * Returns path to XDG configuration directory, if it can be determined.
   */
  inline std::optional<std::filesystem::path>
  config_dir()
  {
    if (const auto env_variable = std::getenv("XDG_CONFIG_HOME"))
    {
      return env_variable;
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
  inline std::vector<std::filesystem::path>
  all_config_dirs()
  {
    auto result = internal::get_multiple_dirs("XDG_CONFIG_DIRS");

#if !defined(_WIN32)
    if (result.empty())
    {
      result.push_back("/etc/xdg");
    }
#endif

    return result;
  }

  /**
   * Returns path to XDG state directory, if it can be determined.
   */
  inline std::optional<std::filesystem::path>
  state_dir()
  {
    if (const auto env_variable = std::getenv("XDG_STATE_HOME"))
    {
      return env_variable;
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
  inline std::optional<std::filesystem::path>
  cache_dir()
  {
    if (const auto env_variable = std::getenv("XDG_CACHE_HOME"))
    {
      return env_variable;
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
  inline std::optional<std::filesystem::path>
  runtime_dir()
  {
    if (const auto env_variable = std::getenv("XDG_RUNTIME_DIR"))
    {
      return env_variable;
    }

    return std::nullopt;
  }
}
