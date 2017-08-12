/*!
  \file simple_nanairo.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <fstream>
#include <memory>
#include <string>
// cxxopts
#include "cxxopts.hpp"
// Nanairo
#include "simple_renderer.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"

/*!
  */
struct NanairoParameters
{
  std::string nanabin_file_path_ = " ";
};

//! Process command line arguments
void processCommandLine(int& argc, char** argv, NanairoParameters* parameters);

//! Load Nanairo binary file
std::ifstream loadNanabin(const std::string& nanabin_file_path);

int main(int argc, char** argv)
{
  std::unique_ptr<nanairo::SimpleRenderer> renderer;
  {
    // Process command line
    NanairoParameters parameters;
    processCommandLine(argc, argv, &parameters);
    // Load nanairo binary file
    auto nanabin = loadNanabin(parameters.nanabin_file_path_);
    // Load scene settings
    nanairo::SceneSettingNode settings;
    settings.readData(&nanabin);
    // Initialize renderer
    renderer = std::make_unique<nanairo::SimpleRenderer>(settings);
  }

  renderer->renderImage();

  return 0;
}

/*!
  */
void processCommandLine(int& argc, char** argv, NanairoParameters* parameters)
{
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
  }
  catch (const cxxopts::OptionException& error) {
    std::cerr << "Error: " << error.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

/*!
  */
std::ifstream loadNanabin(const std::string& nanabin_file_path)
{
  std::ifstream nanabin{nanabin_file_path};
  if (!nanabin.is_open())
    std::cerr << "Error: \"" << nanabin_file_path << "\" not found." << std::endl;
  return nanabin;
}
