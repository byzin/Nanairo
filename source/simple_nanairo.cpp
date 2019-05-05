/*!
  \file simple_nanairo.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
// cxxopts
#include "cxxopts.hpp"
// Nanairo
#include "simple_renderer.hpp"
#include "simple_progress_bar.hpp"
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
  std::unique_ptr<std::ofstream> log_stream;
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
    log_stream = nanairo::makeTextLogStream(parameters->output_path_);
    renderer->setLogStream(log_stream.get());
    std::string error_message;
    const bool is_runnable = renderer->loadScene(settings, &error_message);
    if (!is_runnable) {
      std::cerr << "Scene loading error: " << error_message;
      exit(EXIT_FAILURE);
    }
    output_path = std::move(parameters->output_path_);
  }

  // Make a progress bar 
  nanairo::SimpleProgressBar progress_bar;
  auto notify_of_progress =
  [&progress_bar](const double progress, const std::string_view status)
  {
    progress_bar.update(progress, status);
  };
  renderer->setProgressCallback(notify_of_progress);

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
    options.parse_positional({"binpath"});
    auto result = options.parse(argc, argv);

    // Process command line arguments
    if (0 < result.count("help")) {
      std::cout << options.help({""}) << std::endl;
      exit(EXIT_SUCCESS);
    }
    if (result.count("binpath") == 0) {
      std::cout << options.help({""}) << std::endl;
      exit(EXIT_SUCCESS);
    }
    if (result.count("outputpath") == 0) {
      parameters->output_path_.clear();
      parameters->output_path_ = ".";
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
