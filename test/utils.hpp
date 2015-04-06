#pragma once
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include "unittest.hpp"
#define RUN(a, b) UNITTEST_RUN(a, b)
#define SPOT UNITTEST_SPOT


namespace utils {

const float feps = std::numeric_limits<float>::epsilon();

const double deps = std::numeric_limits<double>::epsilon();

struct mytestcase : unittest::testcase<> {
protected:

	void assert_file_exists(const std::string& file) const {
		if (!std::ifstream(file).good())
			fail(unittest::join("file does not exist: ", file));
	}

	void assert_files_exist(const std::vector<std::string>& file_list) const {
		for (auto file : file_list)
			assert_file_exists(file);
	}

	void assert_file_doesnt_exist(const std::string& file) const {
		if (std::ifstream(file).good())
			fail(unittest::join("file exists: ", file));
	}

	void assert_files_dont_exist(const std::vector<std::string>& file_list) const {
        for (auto file : file_list)
			assert_file_doesnt_exist(file);
	}

};

}
