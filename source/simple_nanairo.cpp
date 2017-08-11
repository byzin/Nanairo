/*!
  \file simple_nanairo.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// cxxopts
#include "cxxopts.hpp"

//! Process command line arguments
void processCommandLine(int& argc, char** argv);

int main(int argc, char** argv)
{
  processCommandLine(argc, argv);

  return 0;
}

/*!
  */
void processCommandLine(int& argc, char** argv)
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
      options.add_options("Argument")
          ("binpath", "Specify nanabin file path.");
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
    std::cerr << "Command line error: " << error.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}
