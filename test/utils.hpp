#pragma once
#include <vector>
#include <string>
#include <limits>
#include "unittest.hpp"
#define RUN(a, b) UNITTEST_RUN(a, b)
#define SPOT UNITTEST_SPOT


namespace utils {

const float feps = std::numeric_limits<float>::epsilon();

const double deps = std::numeric_limits<double>::epsilon();

inline void remove_files_safely(const std::vector<std::string>& file_list) {
    for (auto file : file_list) {
		const auto name = file.c_str();
		if (access(name, W_OK)==0) remove(name);
	}
}

struct mytestcase : unittest::testcase<> {
protected:

	void assert_file_exists(const std::string& file) const {
		if (access(file.c_str(), F_OK)!=0)
			fail(unittest::join("file does not exist: ", file));
	}

	void assert_files_exist(const std::vector<std::string>& file_list) const {
		for (auto file : file_list)
			assert_file_exists(file);
	}

	void assert_file_doesnt_exist(const std::string& file) const {
		if (access(file.c_str(), F_OK)==0)
			fail(unittest::join("file exists: ", file));
	}

	void assert_files_dont_exist(const std::vector<std::string>& file_list) const {
        for (auto file : file_list)
			assert_file_doesnt_exist(file);
	}

};

}
