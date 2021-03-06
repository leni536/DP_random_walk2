#include <iostream>

#include <yaml-cpp/yaml.h>
#include <armadillo>

#include "Measurement.h"
#include "globals.h"
#include "cli_parser.h"

int main(int argc, const char* argv[]) try {
	globals::options = cli_parser::parse(argc - 1, argv + 1);
	YAML::Node node = YAML::LoadFile("Measurement.yaml");
	auto measurement_uptr = node.as<std::unique_ptr<Measurement::Base>>();
	measurement_uptr->run();
	return 0;
} catch (const YAML::Exception& e) {
	std::cerr << "YAML Parsing error at line " << e.mark.line << std::endl;
	throw;
}
