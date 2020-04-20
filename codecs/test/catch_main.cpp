#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> testFilePaths{};

int main(int argc, char* argv[])
{
  Catch::Session session; // There must be exactly one instance

  // Build a new parser on top of Catch's
  using namespace Catch::clara;
  auto cli = session.cli() | Opt(testFilePaths, "test files")["--test-file"](
                                 "path to a test_file");

  // Now pass the new composite back to Catch so it uses that
  session.cli(cli);

  // Let Catch (using Clara) parse the command line
  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0) // Indicates a command line error
    return returnCode;

  for (auto const& path : testFilePaths)
    std::cout << "test file: " << path << std::endl;

  return session.run();
}
