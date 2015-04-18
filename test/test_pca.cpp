/**
 * @file test_pca.cpp
 * @brief Unit tests for the class stats::pca
 */
#include "test_pca.h"

using namespace std;

test_pca::test_pca() {
    srand(1);
}

void test_pca::add_records(stats::pca& pca) {
	const std::vector<double> record1 = {1, 2.5, 42, 7};
	const std::vector<double> record2 = {3, 4.2, 90, 7};
	const std::vector<double> record3 = {456, 444, 0, 7};
	pca.add_record(record1);
	pca.add_record(record2);
	pca.add_record(record3);
}

void test_pca::test_set_num_variables() {
	long exp;

	exp = 5;
	stats::pca pca1;
	pca1.set_num_variables(exp);
	assert_equal(exp, pca1.get_num_variables(), SPOT);
	stats::pca pca2(exp);
	assert_equal(exp, pca2.get_num_variables(), SPOT);

	exp = 0;
	stats::pca pca3;
	assert_throw<std::invalid_argument>([&]() { pca3.set_num_variables(exp); }, SPOT);

	struct Functor {
	    void operator()(long arg1) {
		new stats::pca(arg1);
	}} functor;
	assert_throw<std::invalid_argument>([&]() { functor(exp); }, SPOT);

	exp = 1;
	stats::pca pca5;
	assert_throw<std::invalid_argument>([&]() { pca5.set_num_variables(exp); }, SPOT);
	assert_throw<std::invalid_argument>([&]() { functor(exp); }, SPOT);

	exp = 2;
	stats::pca pca7;
	assert_no_throw([&]() { pca7.set_num_variables(exp); }, SPOT);
	assert_no_throw([&]() { functor(exp); }, SPOT);
}

void test_pca::test_add_record() {
	const std::vector<double> record1 = {1, 2.5, 42, 7};
	const std::vector<double> record2 = {3, 4.2, 90, 7};
	const std::vector<double> record3 = {456, 444, 0, 7};

	stats::pca pca(4);
	pca.add_record(record1);
	pca.add_record(record2);
	pca.add_record(record3);

	const long exp = 3;
	assert_equal(exp, pca.get_num_records(), SPOT);

	const auto rec1 = pca.get_record(0);
	assert_equal_containers(record1, rec1, SPOT);

	const auto rec2 = pca.get_record(1);
	assert_equal_containers(record2, rec2, SPOT);

	const auto rec3 = pca.get_record(2);
	assert_equal_containers(record3, rec3, SPOT);

	std::vector<double> record4 = {4, 8, 7};

	assert_throw<std::domain_error>([&]() { pca.add_record(record4); }, SPOT);
}

void test_pca::test_set_do_normalize() {
	stats::pca pca;
	bool exp = false;
	assert_equal(exp, pca.get_do_normalize(), SPOT);
	exp = true;
	pca.set_do_normalize(exp);
	assert_equal(exp, pca.get_do_normalize(), SPOT);
}

void test_pca::test_set_do_bootstrap() {
	stats::pca pca;
	assert_false(pca.get_do_bootstrap(), SPOT);

	const bool exp = true;
	pca.set_do_bootstrap(exp);
	assert_equal(exp, pca.get_do_bootstrap(), SPOT);
	assert_equal(pca.get_num_bootstraps(), 30, SPOT);
	assert_equal(pca.get_bootstrap_seed(), 1, SPOT);

	const long num_boots = 9;
	const long seed = 1;
	assert_throw<std::invalid_argument>([&]() { pca.set_do_bootstrap(exp, num_boots, seed); }, SPOT);
}

void test_pca::test_set_solver() {
	stats::pca pca;
	assert_equal(pca.get_solver(), "dc", SPOT);
	std::string exp = "standard";
	pca.set_solver(exp);
	assert_equal(exp, pca.get_solver(), SPOT);
	exp = "dc";
	pca.set_solver(exp);
	assert_equal(exp, pca.get_solver(), SPOT);
	std::string solver = "java_sucks";
	assert_throw<std::logic_error>([&]() { pca.set_solver(solver); }, SPOT);
}

void test_pca::test_solve_throws() {
	const std::vector<double> record1 = {1, 2.5, 42, 7};
	const int nvar = 4;
	stats::pca pca(nvar);
	pca.add_record(record1);
	assert_throw<std::logic_error>([&]() { pca.solve(); }, SPOT);
}

void test_pca::test_save() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);

	pca.set_do_bootstrap(true);

	pca.solve();


	pca.save("test_data");

	const std::vector<std::string> tmp_files{
		"test_data.eigval","test_data.eigvalboot","test_data.eigvec",
		"test_data.energy","test_data.energyboot","test_data.mean",
		"test_data.pca","test_data.princomp","test_data.sigma"};
	assert_files_exist(tmp_files);

	stats::pca pca_loaded;
	pca_loaded.load("test_data");
	assert_true(pca==pca_loaded, SPOT);
}

void test_pca::test_check_projection_accurate_dc() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);

	pca.set_solver("dc");

	pca.solve();

	const double projacc = pca.check_projection_accurate();
	const double exp_projacc = 1;
	assert_approx_equal(exp_projacc, projacc, utils::feps, SPOT);
}

void test_pca::test_check_projection_accurate_standard() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);

	pca.set_solver("standard");

	pca.solve();

	const double projacc = pca.check_projection_accurate();
	const double exp_projacc = 1;
	assert_approx_equal(exp_projacc, projacc, utils::feps, SPOT);
}

void test_pca::test_eigenvectors() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);
	pca.solve();

	const auto eigvec1 = pca.get_eigenvector(0);
	const vector<double> exp_eigvec1 = {0.7136892, 0.69270403, -0.10396568, 0};
	assert_approx_equal_containers(exp_eigvec1, eigvec1, utils::feps, SPOT);

	const auto eigvec2 = pca.get_eigenvector(1);
	const vector<double> exp_eigvec2 = {0.07711363, 0.06982266, 0.99457442, 0};
	assert_approx_equal_containers(exp_eigvec2, eigvec2, utils::feps, SPOT);

	const auto eigvec3 = pca.get_eigenvector(2);
	const vector<double> exp_eigvec3 = {0, 0, 0, 1};
	assert_approx_equal_containers(exp_eigvec3, eigvec3, utils::feps, SPOT);

	const auto eigvec4 = pca.get_eigenvector(3);
	const vector<double> exp_eigvec4 = {-0.69620487, 0.71783419, 0.00358524, 0};
	assert_approx_equal_containers(exp_eigvec4, eigvec4, utils::feps, SPOT);
}

void test_pca::test_principals() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);
	pca.solve();

	const float factor = 10;
	const auto prinvec1 = pca.get_principal(0);
	const vector<double> exp_prinvec1 = {-2.10846198e+02, -2.13231575e+02, 4.24077773e+02};
	assert_approx_equal_containers(exp_prinvec1, prinvec1, utils::feps*factor, SPOT);

	const auto prinvec2 = pca.get_principal(1);
	const vector<double> exp_prinvec2 = {-2.40512596e+01, 2.39612385e+01, 9.00211615e-02};
	assert_approx_equal_containers(exp_prinvec2, prinvec2, utils::feps*factor, SPOT);

	const auto prinvec3 = pca.get_principal(2);
	const vector<double> exp_prinvec3 = {0, 0, 0};
	assert_approx_equal_containers(exp_prinvec3, prinvec3, utils::feps*factor, SPOT);

	const auto prinvec4 = pca.get_principal(3);
	const vector<double> exp_prinvec4 = {0, 0, 0};
	assert_approx_equal_containers(exp_prinvec4, prinvec4, utils::feps*factor, SPOT);
}

void test_pca::test_eigenvalues() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);
	pca.set_do_bootstrap(true, 10, 1);
	pca.solve();

	const auto eigval = pca.get_eigenvalues();
	const vector<double> exp_eigval = {9.95745538e-01, 4.25446249e-03, 0, 0};
	assert_approx_equal_containers(exp_eigval, eigval, utils::feps, SPOT);

	for (int i=0; i<nvar; ++i) {
		const size_t exp_size = 10;
		assert_equal(exp_size, pca.get_eigenvalue_boot(i).size(), SPOT);
	}
}

void test_pca::test_energy() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);
	pca.set_do_bootstrap(true, 10, 1);
	pca.solve();

	const double energy = pca.get_energy();
	const double exp_energy = 135459.19666667;
	assert_approx_equal(exp_energy, energy, utils::feps, SPOT);

	const size_t exp_size = 10;
	assert_equal(exp_size, pca.get_energy_boot().size(), SPOT);
}

void test_pca::test_check_eigenvectors_orthogonal() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);
	pca.solve();

	const double orthog = pca.check_eigenvectors_orthogonal();
	const double exp_orthog = 1;
	assert_approx_equal(exp_orthog, orthog, utils::feps, SPOT);
}

void test_pca::test_projections_to_space() {
	const int nvar = 4;
	stats::pca pca(nvar);
	add_records(pca);
	pca.solve();

	const auto record1 = pca.get_record(0);
	const auto prin1 = pca.to_principal_space(record1);
	const auto rec1 = pca.to_variable_space(prin1);
	assert_approx_equal_containers(record1, rec1, utils::feps, SPOT);

	const auto record2 = pca.get_record(1);
	const auto prin2 = pca.to_principal_space(record2);
	const auto rec2 = pca.to_variable_space(prin2);
	assert_approx_equal_containers(record2, rec2, utils::feps, SPOT);

	const auto record3 = pca.get_record(2);
	const auto prin3 = pca.to_principal_space(record3);
	const auto rec3 = pca.to_variable_space(prin3);
	assert_approx_equal_containers(record3, rec3, utils::feps, SPOT);
}
