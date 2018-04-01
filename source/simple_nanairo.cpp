/*!
  \file simple_nanairo.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <fstream>
#include <memory>
#include <string>
#include <utility>
// cxxopts
#include "cxxopts.hpp"
// Nanairo
#include "simple_renderer.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"

namespace {

/*!
  */
struct NanairoParameters
{
  std::string nanabin_file_path_ = " ";
  std::string output_path_ = ".";
};

//! Process command line arguments
std::unique_ptr<NanairoParameters> processCommandLine(int& argc, char** argv);

//! Load Nanairo binary file
std::ifstream loadSceneBinary(const std::string& nanabin_file_path);

}

int main(int argc, char** argv)
{
  std::string output_path;
  std::unique_ptr<nanairo::SimpleRenderer> renderer;
  {
    // Process command line
    auto parameters = ::processCommandLine(argc, argv);
    // Load nanairo binary file
    auto nanabin = ::loadSceneBinary(parameters->nanabin_file_path_);
    // Load scene settings
    nanairo::SceneSettingNode settings;
    settings.readData(&nanabin);
    // Initialize renderer
    renderer = std::make_unique<nanairo::SimpleRenderer>();
    std::string error_message;
    const bool is_runnable = renderer->loadScene(settings, &error_message);
    if (!is_runnable) {
      std::cerr << "Scene loading error: " << error_message;
      exit(EXIT_FAILURE);
    }
    output_path = std::move(parameters->output_path_);
  }
  // Start rendering
  renderer->render(output_path);

  return 0;
}

namespace {

/*!
  */
std::unique_ptr<NanairoParameters> processCommandLine(int& argc, char** argv)
{
  auto parameters = std::make_unique<NanairoParameters>();

  try {
    cxxopts::Options options{
        argv[0],
        "Nanairo is a physically plausible spectral renderer."};
    options.positional_help("<nanabin>");

    // Add options
    // Help
    {
      options.add_options()
          ("h,help", "Display this help.");
    }
    {
      auto value = cxxopts::value(parameters->nanabin_file_path_);
      options.add_options("Argument")
          ("binpath", "Specify nanabin file path.", value);
    }
    {
      auto value = cxxopts::value(parameters->output_path_);
      options.add_options()
          ("o,outputpath", "Specify the output dir in which images are saved.", value);
    }

    // Parse command line
    options.parse_positional(std::vector<std::string>{"binpath"});
    options.parse(argc, argv);

    // Process command line arguments
    if (0 < options.count("help")) {
      std::cout << options.help(std::vector<std::string>{""}) << std::endl;
      exit(EXIT_SUCCESS);
    }
    if (options.count("binpath") == 0) {
      std::cout << options.help(std::vector<std::string>{""}) << std::endl;
      exit(EXIT_SUCCESS);
    }
    if (options.count("outputpath") == 0) {
      parameters->output_path_.clear();
    }
  }
  catch (const cxxopts::OptionException& error) {
    std::cerr << "Error: " << error.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return parameters;
}

/*!
  */
std::ifstream loadSceneBinary(const std::string& nanabin_file_path)
{
  std::ifstream nanabin{nanabin_file_path, std::ios::binary};
  if (!nanabin.is_open())
    std::cerr << "Error: \"" << nanabin_file_path << "\" not found." << std::endl;
  return nanabin;
}

}
