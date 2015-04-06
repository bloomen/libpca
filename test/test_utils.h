#pragma once
/**
 * @file test_utils.h
 * @brief Unit tests for everything under the namespace stats::utils
 */
#include "pca.h"
#include "utils.hpp"


struct test_utils : utils::mytestcase {

	static void run() {
		RUN(test_utils, test_make_covariance_matrix)
		RUN(test_utils, test_make_shuffled_matrix)
		RUN(test_utils, test_compute_column_means)
		RUN(test_utils, test_remove_column_means)
		RUN(test_utils, test_compute_column_rms)
		RUN(test_utils, test_normalize_by_column)
		RUN(test_utils, test_enforce_positive_sign_by_column)
		RUN(test_utils, test_extract_column_vector)
		RUN(test_utils, test_extract_column_vector_throws)
		RUN(test_utils, test_extract_row_vector)
		RUN(test_utils, test_extract_row_vector_throws)
		RUN(test_utils, test_assert_file_good)
		RUN(test_utils, test_remove_column_means_throws)
		RUN(test_utils, test_normalize_by_column_throws)
		RUN(test_utils, test_write_matrix_object)
		RUN(test_utils, test_read_matrix_object)
		RUN(test_utils, test_is_approx_equal)
		RUN(test_utils, test_is_approx_equal_container)
		RUN(test_utils, test_is_equal_container)
		RUN(test_utils, test_get_mean)
		RUN(test_utils, test_get_sigma)
		RUN(test_utils, test_join)
	}

    test_utils();

	void test_make_covariance_matrix();
	void test_make_shuffled_matrix();
	void test_compute_column_means();
	void test_remove_column_means();
	void test_compute_column_rms();
	void test_normalize_by_column();
	void test_enforce_positive_sign_by_column();
	void test_extract_column_vector();
	void test_extract_column_vector_throws();
	void test_extract_row_vector();
	void test_extract_row_vector_throws();
	void test_assert_file_good();
	void test_remove_column_means_throws();
	void test_normalize_by_column_throws();
	void test_write_matrix_object();
	void test_read_matrix_object();
	void test_is_approx_equal();
	void test_is_approx_equal_container();
	void test_is_equal_container();
	void test_get_mean();
	void test_get_sigma();
	void test_join();

};
