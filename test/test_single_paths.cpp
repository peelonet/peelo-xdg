#include <cassert>
#include <cstdlib>
#include <cstring>
#include <functional>

#include <peelo/xdg.hpp>

using std::filesystem::path;
using std::nullopt;
using std::optional;

using callback_type = std::function<std::optional<std::filesystem::path>()>;

static void
test_with_env_variable(
  const callback_type& callback,
  const char* env_variable_name
)
{
  setenv(env_variable_name, "xdg/test", 1);

  const auto result = callback();

  assert(!!result);
  assert(*result == path("xdg") / "test");
}

static void
test_without_env_variable(
  const callback_type& callback,
  const char* env_variable_name,
  const optional<path>& expected_result
)
{
  unsetenv(env_variable_name);
  setenv("HOME", "xdg", 1);

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
  const char* env_variable_name,
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
}
