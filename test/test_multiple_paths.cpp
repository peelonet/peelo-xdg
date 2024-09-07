#include <cassert>
#include <cstdlib>
#include <functional>

#if defined(_WIN32)
# include <windows.h>
#endif

#include <peelo/xdg.hpp>

using callback_type = std::function<std::vector<std::filesystem::path>()>;

static void
test_callback(
  const callback_type& callback,
  const std::string& env_variable_name
)
{
  using peelo::xdg::internal::path_separator;
  const auto value = std::string("/xdg") + path_separator + path_separator + "/xdg/xdg";

#if defined(_WIN32)
  _putenv((env_variable_name + "=" + value).c_str());
#else
  unsetenv(env_variable_name.c_str());
  setenv(env_variable_name.c_str(), value.c_str(), 1);
#endif

  const auto result = callback();

  assert(result.size() == 2);
  assert(result[0] == std::filesystem::path("/xdg"));
  assert(result[1] == std::filesystem::path("/xdg/xdg"));
}

int
main()
{
  using namespace peelo::xdg;

  test_callback(all_data_dirs<std::filesystem::path>, "XDG_DATA_DIRS");
  test_callback(all_config_dirs<std::filesystem::path>, "XDG_CONFIG_DIRS");
}
