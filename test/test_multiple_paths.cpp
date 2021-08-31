#include <cassert>
#include <cstdlib>
#include <functional>

#include <peelo/xdg.hpp>

#if defined(_WIN32)
static const char separator = ';';
#else
static const char separator = ':';
#endif

using callback_type = std::function<std::vector<std::filesystem::path>()>;

static void test_callback(
  const callback_type& callback,
  const char* env_variable_name
)
{
  unsetenv(env_variable_name);
  setenv(
    env_variable_name,
    (std::string("/xdg") + separator + "/xdg/xdg").c_str(),
    1
  );

  const auto result = callback();

  assert(result.size() == 2);
  assert(result[0] == std::filesystem::path("/xdg"));
  assert(result[1] == std::filesystem::path("/xdg/xdg"));
}

int main()
{
  using namespace peelo::xdg;

  test_callback(all_data_dirs, "XDG_DATA_DIRS");
  test_callback(all_config_dirs, "XDG_CONFIG_DIRS");

  return EXIT_SUCCESS;
}
