**libpca** is a C++ library for principal component analysis and related 
transformations. It comes with example and unit tests. libpca is successfully 
tested on Linux and MacOSX using g++ (>=4.6), clang++ (>=3.2), and icc (>=14.0).

libpca requires Armadillo (>=3.2.4) which can be obtained as a pre-compiled 
package on most distributions or directly from http://arma.sourceforge.net.


Install
-------

```
./configure
make
make install
```

Features
--------

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
- a great deal of libpca runs in parallel thanks to Armadillo
