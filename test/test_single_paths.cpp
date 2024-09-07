#include <cassert>
#include <cstdlib>
#include <cstring>
#include <functional>

#if defined(_WIN32)
# include <windows.h>
#endif

#include <peelo/xdg.hpp>

using std::filesystem::path;
using std::nullopt;
using std::optional;

using callback_type = std::function<std::optional<std::filesystem::path>()>;

static void
test_with_env_variable(
  const callback_type& callback,
  const std::string& env_variable_name
)
{
#if defined(_WIN32)
  _putenv((env_variable_name + "=" + "xdg\\test").c_str());
#else
  setenv(env_variable_name.c_str(), "xdg/test", 1);
#endif

  const auto result = callback();

  assert(!!result);
  assert(*result == path("xdg") / "test");
}

static void
test_without_env_variable(
  const callback_type& callback,
  const std::string& env_variable_name,
  const optional<path>& expected_result
)
{
#if defined(_WIN32)
  _putenv((env_variable_name + "=").c_str());
  _putenv("HOME=xdg");
#else
  unsetenv(env_variable_name.c_str());
  setenv("HOME", "xdg", 1);
#endif

  const auto result = callback();

  if (expected_result)
  {
    assert(!!result);
    assert(*result == expected_result);
  } else {
    assert(!result);
  }
}

static void
test_callback(
  const callback_type& callback,
  const std::string& env_variable_name,
  const optional<path>& expected_result_without_env_var = nullopt
)
{
  test_with_env_variable(callback, env_variable_name);
  test_without_env_variable(
    callback,
    env_variable_name,
    expected_result_without_env_var
  );
}

#if defined(_WIN32)
static void
test_home_dir_with_userprofile()
{
  _putenv("HOME=");
  _putenv("USERPROFILE=C:\\test");

  assert(peelo::xdg::home_dir() == path("C:\\test"));
}

static void
test_home_dir_with_home_drive_and_path()
{
  _putenv("HOME=");
  _putenv("HOMEDRIVE=C:\\");
  _putenv("HOMEPATH=test");

  assert(peelo::xdg::home_dir() == path("C:\\test"));
}
#endif

int
main()
{
  using namespace peelo::xdg;
  const path base = "xdg";

  test_callback(
    data_dir<std::filesystem::path>,
    "XDG_DATA_HOME",
    base / ".local" / "share"
  );
  test_callback(
    config_dir<std::filesystem::path>,
    "XDG_CONFIG_HOME",
    base / ".config"
  );
  test_callback(
    state_dir<std::filesystem::path>,
    "XDG_STATE_HOME",
    base / ".local" / "state"
  );
  test_callback(
    cache_dir<std::filesystem::path>,
    "XDG_CACHE_HOME",
    base / ".cache"
  );
  test_callback(
    runtime_dir<std::filesystem::path>,
    "XDG_RUNTIME_DIR"
  );
#if defined(_WIN32)
  test_home_dir_with_userprofile();
  test_home_dir_with_home_drive_and_path();
#endif
}
