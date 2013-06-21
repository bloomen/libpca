#include "test_pca.h"
#include "test_utils.h"

void unittest::run_all_tests() {
	unittest::call<test_pca>();
	unittest::call<test_utils>();
}
