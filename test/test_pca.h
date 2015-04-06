#pragma once
/**
 * @file test_pca.h
 * @brief Unit tests for the class stats::pca
 */
#include "pca.h"
#include "utils.hpp"


struct test_pca : utils::mytestcase {

	static void run() {
		RUN(test_pca, test_set_num_variables)
		RUN(test_pca, test_add_record)
		RUN(test_pca, test_set_do_normalize)
		RUN(test_pca, test_set_do_bootstrap)
		RUN(test_pca, test_set_solver)
		RUN(test_pca, test_solve_throws)
		RUN(test_pca, test_save)
		RUN(test_pca, test_check_projection_accurate_dc)
		RUN(test_pca, test_check_projection_accurate_standard)
		RUN(test_pca, test_eigenvectors)
		RUN(test_pca, test_principals)
		RUN(test_pca, test_eigenvalues)
		RUN(test_pca, test_energy)
		RUN(test_pca, test_check_eigenvectors_orthogonal)
		RUN(test_pca, test_projections_to_space)
	}

    test_pca();

	void test_set_num_variables();
	void test_add_record();
	void test_set_do_normalize();
	void test_set_do_bootstrap();
	void test_set_solver();
	void test_solve_throws();
	void test_save();
	void test_check_projection_accurate_dc();
	void test_check_projection_accurate_standard();
	void test_eigenvectors();
	void test_principals();
	void test_eigenvalues();
	void test_energy();
	void test_check_eigenvectors_orthogonal();
	void test_projections_to_space();

    void add_records(stats::pca& pca);
};
