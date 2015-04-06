/**
 * @file pca_example.cpp
 * @brief This is an example program demonstrating the usage of the pca class
 */
#include <pca.h>
#include <iostream>
#include <random>

using namespace std;

int main() {

	const int num_variables = 10;
	const int num_records = 300;

	stats::pca pca(num_variables);
	pca.set_do_bootstrap(true, 100);

	cout<<"Adding random data records ..."<<endl;
	srand(1);
	for (int i=0; i<num_records; ++i) {
		vector<double> record(num_variables);
		for (auto& value : record) {
			value = rand()%20 - 10;
		}
		pca.add_record(record);
	}

	cout<<"Solving ..."<<endl;
	pca.solve();

	cout<<"Energy = "<<pca.get_energy()<<" ("<<
      		stats::utils::get_sigma(pca.get_energy_boot())<<")"<<endl;

	const auto eigenvalues = pca.get_eigenvalues();
	cout<<"First three eigenvalues = "<<eigenvalues[0]<<", "
									  <<eigenvalues[1]<<", "
									  <<eigenvalues[2]<<endl;

	cout<<"Orthogonal Check = "<<pca.check_eigenvectors_orthogonal()<<endl;
	cout<<"Projection Check = "<<pca.check_projection_accurate()<<endl;

	pca.save("pca_results");

	return 0;
}
