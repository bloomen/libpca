/**
 * @file test_utils.cpp
 * @brief Unit tests for everything under the namespace stats::utils
 */
#include "test_utils.h"

using namespace std;
using namespace stats::utils;

test_utils::test_utils() {
    srand(1);
}

void test_utils::test_make_covariance_matrix() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const auto result = make_covariance_matrix(data);
	const auto exp = arma::Mat<double>(1./2 * data.t() * data);
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_make_shuffled_matrix() {
	const vector<double> vec = {4,1,1,2,5,2,3,3,6};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const auto result = make_shuffled_matrix(data);
	const vector<double> vec2 = {1,1,4,5,2,5,3,3,3};
	const arma::Mat<double> exp(&vec2.front(), 3, 3);
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_compute_column_means() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const auto result = compute_column_means(data);
	const arma::Col<double> exp = {2, 5, 8};
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_remove_column_means() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	arma::Mat<double> result(&vec.front(), 3, 3);
	const auto means = compute_column_means(result);
	remove_column_means(result, means);
	const vector<double> vec2 = {-1,0,1,-1,0,1,-1,0,1};
	const arma::Mat<double> exp(&vec2.front(), 3, 3);
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_remove_column_means_throws() {
	arma::Mat<double> data(3, 3);
	const arma::Col<double> means(2);
	assert_throw<std::range_error>([&]() { remove_column_means(data, means); }, SPOT);
}

void test_utils::test_compute_column_rms() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const auto result = compute_column_rms(data);
	const arma::Col<double> exp = {sqrt(7), sqrt(38.5), sqrt(97)};
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_normalize_by_column() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	arma::Mat<double> result(&vec.front(), 3, 3);
	const auto sigmas = compute_column_rms(result);
	normalize_by_column(result, sigmas);
	const double first = 1./sqrt(7);
	const double second = 1./sqrt(38.5);
	const double third = 1./sqrt(97);
	const vector<double> vec2 = {1*first,2*first,3*first,
								 4*second,5*second,6*second,
								 7*third,8*third,9*third};
	const arma::Mat<double> exp(&vec2.front(), 3, 3);
	assert_approx_equal_containers(exp, result, utils::feps, SPOT);
}

void test_utils::test_normalize_by_column_throws() {
	arma::Mat<double> data(3, 3);
	const arma::Col<double> sigmas1(2);
	assert_throw<std::range_error>([&]() { normalize_by_column(data, sigmas1); }, SPOT);
	const arma::Col<double> sigmas2 = {0, 0, 0};
	assert_throw<std::runtime_error>([&]() { normalize_by_column(data, sigmas2); }, SPOT);
}

void test_utils::test_enforce_positive_sign_by_column() {
	const vector<double> vec = {1,2,3,4,5,-6,7,8,-9};
	arma::Mat<double> result(&vec.front(), 3, 3);
	enforce_positive_sign_by_column(result);
	const vector<double> vec2 = {1,2,3,-4,-5,6,-7,-8,9};
	const arma::Mat<double> exp(&vec2.front(), 3, 3);
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_extract_column_vector() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const auto result = extract_column_vector(data, 1);
	const vector<double> exp = {4,5,6};
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_extract_column_vector_throws() {
	const arma::Mat<double> data(3, 3);
	const int index = 3;
	assert_throw<std::range_error>([&]() { extract_column_vector(data, index); }, SPOT);
}

void test_utils::test_extract_row_vector() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const auto result = extract_row_vector(data, 1);
	const vector<double> exp = {2,5,8};
	assert_equal_containers(exp, result, SPOT);
}

void test_utils::test_extract_row_vector_throws() {
	const arma::Mat<double> data(3, 3);
	const int index = 3;
	assert_throw<std::range_error>([&]() { extract_row_vector(data, index); }, SPOT);
}

void test_utils::test_assert_file_good() {
	const string filename = "dummy";
	bool flag = false;
	assert_throw<std::ios_base::failure>([&]() { assert_file_good(flag, filename); }, SPOT);
	flag = true;
	assert_no_throw([&]() { assert_file_good(flag, filename); }, SPOT);
}

void test_utils::test_write_matrix_object() {
	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	const string filename = "test_data.matrix_write";
	write_matrix_object(filename, data);
	assert_file_exists(filename);

	const string filename2 = "nada/test_matrix";
	assert_throw<std::ios_base::failure>([&]() { write_matrix_object<arma::Mat<double>>(filename2, data); }, SPOT);
}

void test_utils::test_read_matrix_object() {
	arma::Mat<double> tmp;
	const string filename = "test_data.matrix_read";
	assert_throw<std::ios_base::failure>([&]() { read_matrix_object<arma::Mat<double>>(filename + "_not_here", tmp); }, SPOT);

	const vector<double> vec = {1,2,3,4,5,6,7,8,9};
	const arma::Mat<double> data(&vec.front(), 3, 3);
	write_matrix_object(filename, data);
	arma::Mat<double> result;
	read_matrix_object(filename, result);
	assert_equal_containers(data, result, SPOT);
}

void test_utils::test_is_approx_equal() {
	const double val1 = 1;
	const double val2 = 1.01;
	const double eps1 = 0.02;
	assert_true(stats::utils::is_approx_equal(val1, val2, eps1), SPOT);
	const double val3 = 1.02;
	assert_false(stats::utils::is_approx_equal(val1, val3, eps1), SPOT);
}

void test_utils::test_is_approx_equal_container() {
	const vector<double> vec1 = {1, 2, 3};
	const vector<double> vec2 = {1.01, 2, 3};
	const double eps1 = 0.02;
	assert_true(is_approx_equal_container(vec1, vec2, eps1), SPOT);
}

void test_utils::test_is_equal_container() {
	const vector<double> vec1 = {1, 2, 3};
	const vector<double> vec2 = {1, 2, 3};
	assert_equal_containers(vec1, vec2, SPOT);
}

void test_utils::test_get_mean() {
	const vector<double> vec = {1, 2, 3};
	const double exp = 2;
	assert_equal(exp, get_mean(vec), SPOT);
}

void test_utils::test_get_sigma() {
	const vector<double> vec = {1, 2, 3};
	const double exp = 1;
	assert_equal(exp, get_sigma(vec), SPOT);
}

void test_utils::test_join() {
	const std::string exp1 = "something";
	assert_equal(exp1, join("something"), SPOT);

	const std::string exp2 = "something123cool";
	assert_equal(exp2, join("something", 123, "cool"), SPOT);
}
