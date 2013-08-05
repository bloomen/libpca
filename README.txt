libpca is a C++ library computing a principal component analysis plus 
corresponding transformations. Please check the example and the pca class
interface. libpca has been successfully tested under Linux and GCC4.6.

Features:

- computes a principal component analysis
- computes energy, eigenvalues, eigenvectors, principal components
- option to normalize the data matrix
- option to bootstrap the eigenproblem to obtain uncertainty
	estimates of energy and eigenvalues
- methods to project data records to the space of principal 
	components and back
- method to reduce the number of eigenvectors affecting the
	data record projections
- methods to check the accuracy of the solution to the eigenproblem
- methods to save and load pca properties to and from files
- pca offers a standard C++ interface using not more than 
	primitive types, std::string, and std::vector
- libpca comes with example and unit tests 
- a great deal of pca runs in parallel thanks to Armadillo


libpca uses Armadillo (>=3.2.4) which can be obtained as a package on most
distributions or from http://arma.sourceforge.net. In case Armadillo is 
not installed in a standard location ensure that all environment variables 
(CPLUS_INCLUDE_PATH, LIBRARY_PATH, LD_LIBRARY_PATH) are properly set. 

In order to build and install libpca simply unpack the archive, enter the 
resulting directory and execute: 

bash install.sh [install_directory]

If install_directory is not given then libpca will be installed in a standard
system location (/usr/local) requiring root privileges.

libpca is being developed by Christian Blume. Contact Christian at
chr.blume@gmail.com for any questions or comments.
