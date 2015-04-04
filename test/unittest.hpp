#pragma once
/**
 * @brief A C++ library for unit testing
 *        http://sourceforge.net/projects/libunittest
 * @file unittest.hpp
 */
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <tuple>
#include <typeinfo>
#include <iostream>
#include <memory>
#include <chrono>
#include <mutex>
#include <thread>
#include <algorithm>
#include <regex>
#include <functional>
#include <fstream>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Returns the libunittest version as a tuple
 * @returns The version as a tuple
 */
std::tuple<int,int,int>
get_version_tuple();
/**
 * @brief Returns the libunittest version as a string
 * @returns The version as a string
 */
std::string
get_version_string();
/**
 * @brief This function is to be implemented by the user if
 *  make_default_environment() is called. run_all_tests() should
 *  contain a list of calls to test classes
 */
void
run_all_tests();
/**
 * @brief Calls the static run() method of the test class
 */
template<typename TestCase>
void
call()
{
    TestCase::run();
}
/**
 * @brief Calls the static run() method of the test class with the test
 *  context as the one parameter
 * @param context The test context
 */
template<typename TestCase,
         typename TestContext>
void
call(const TestContext& context)
{
    TestCase::run(context);
}
/**
 * @brief High level user arguments to control application behavior
 */
class userargs {
public:
    /**
     * @brief Constructor
     */
    userargs();
    /**
     * @brief Destructor
     */
    virtual ~userargs();
    /**
     * @brief Returns whether the output is verbose
     * @returns Whether the output is verbose
     */
    bool
    verbose() const;
    /**
     * @brief Sets whether the output is verbose
     * @param value The value
     */
    virtual void
    verbose(bool value);
    /**
     * @brief Returns whether to stop running after the first test fails
     * @returns Whether to stop running after first failure
     */
    bool
    failure_stop() const;
    /**
     * @brief Sets whether to stop running after first failure
     * @param value The value
     */
    virtual void
    failure_stop(bool value);
    /**
     * @brief Returns whether to generate XML output
     * @returns Whether to generate XML output
     */
    bool
    generate_xml() const;
    /**
     * @brief Sets whether to generate XML output
     * @param value The value
     */
    virtual void
    generate_xml(bool value);
    /**
     * @brief Returns a name filter on the full test name
     * @returns The filter string
     */
    std::string
    name_filter() const;
    /**
     * @brief Sets a name filter on the full test name
     * @param value The value
     */
    virtual void
    name_filter(const std::string& value);
    /**
     * @brief Returns a certain test to be run. This supersedes the name filter
     * @returns The test name
     */
    std::string
    test_name() const;
    /**
     * @brief Sets a certain test to be run. This supersedes the name filter
     * @param value The value
     */
    virtual void
    test_name(const std::string& value);
    /**
     * @brief Returns the XML output file name
     * @returns The XML output file name
     */
    std::string
    xml_filename() const;
    /**
     * @brief Sets the XML output file name
     * @param value The value
     */
    virtual void
    xml_filename(const std::string& value);

protected:

    bool verbose_;
    bool failure_stop_;
    bool generate_xml_;
    std::string name_filter_;
    std::string test_name_;
    std::string xml_filename_;
};
/**
 * @brief Sets up a default testing environment and calls run_all_tests().
 *  This high-level function is intended to be used directly from the
 *  user's main() function
 * @param argc The number of user arguments
 * @param argv The container of the user arguments
 * @returns A platform dependent program execution status: A success value if
 *  all tests succeed and an error value otherwise
 */
int
make_default_environment(int argc, char **argv);
/**
 * @brief An enumeration for the test status
 */
enum teststatus : unsigned int {
    success = 0,
    failure = 1,
    error = 2,
    skipped = 3
};
/**
 * @brief The exception class to indicate test failures
 */
class testfailure : public std::runtime_error {
public:
    /**
     * @brief Constructor
     * @param message The exception message
     */
    explicit testfailure(const std::string& message);
};
/**
 * @brief An overload doing nothing to finish template recursion
 * @param stream An output stream
 */
void
write_to_stream(std::ostream& stream);
/**
 * @brief Writes arguments to an output stream via template recursion
 * @param stream An output stream
 * @param arg An argument
 * @param args An arbitrary number of arguments
 */
template<typename T,
         typename... Args>
void
write_to_stream(std::ostream& stream,
                const T& arg,
                const Args&... args)
{
    stream << arg;
    write_to_stream(stream, args...);
}
/**
 * @brief Joins an arbitrary number of input arguments to a single string. All
 *  arguments must implement the << operator
 * @param arg An argument
 * @param args An arbitrary number of arguments, can be omitted
 * @returns The joined string
 */
template<typename T,
         typename... Args>
std::string
join(const T& arg,
     const Args&... args)
{
    std::ostringstream stream;
    stream << arg;
    write_to_stream(stream, args...);
    return std::move(stream.str());
}
/**
 * @brief Stores logging info about a single test
 */
struct testlog {
    /**
     * @brief Constructor
     */
    testlog();
    /**
     * @brief The name of the test class
     */
    std::string class_name;
    /**
     * @brief The name of the test method
     */
    std::string test_name;
    /**
     * @brief Whether the test ran successfully
     */
    bool successful;
    /**
     * @brief The test status
     */
    teststatus status;
    /**
     * @brief The name of the error type. Empty if test was successful
     */
    std::string error_type;
    /**
     * @brief The result message
     */
    std::string message;
    /**
     * @brief The test duration in seconds
     */
    double duration;
};
/**
 * @brief Stores the test results
 */
struct testresults {
    /**
     * @brief Constructor
     */
    testresults();
    /**
     * @brief Whether all tests ran successfully
     */
    bool successful;
    /**
     * @brief The number of tests
     */
    long n_tests;
    /**
     * @brief The number of test successes
     */
    long n_successes;
    /**
     * @brief The number of test failures
     */
    long n_failures;
    /**
     * @brief The number of test errors
     */
    long n_errors;
    /**
     * @brief The number of skipped tests
     */
    long n_skipped;
    /**
     * @brief The duration of all tests in seconds
     */
    double duration;
    /**
     * @brief A vector of testlog instances. One instance for each test
     */
    std::vector<testlog> testlogs;
};
/**
 * @brief The test suite controlling the test runs (singleton, thread-safe)
 */
class testsuite {
public:
    /**
     * @brief Returns a pointer to the instance of this class
     * @returns A pointer to the instance of this class
     */
    static testsuite*
    instance();
    /**
     * @brief Sets whether output is verbose for running tests
     * @param verbose Whether output is verbose for running tests
     */
    void
    set_verbose(bool verbose);
    /**
     * @brief Sets whether to stop running after the first test fails
     * @param failure_stop Whether to stop running after first failure
     */
    void
    set_failure_stop(bool failure_stop);
    /**
     * @brief Sets a filter on the full test name. This will make the suite
     *  execute only those tests that begin with the given filter string
     * @param name_filter The filter string
     */
    void
    set_name_filter(const std::string& name_filter);
    /**
     * @brief Sets a certain test to be run. This supersedes the name filter
     * @param test_name The test name
     */
    void
    set_test_name(const std::string& test_name);
    /**
     * @brief Returns the accumulated test results
     * @returns The test results
     */
    testresults
    get_results() const;

private:
    friend class testrunner;

    testsuite();

    testsuite(const testsuite&) = delete;

    testsuite&
    operator=(const testsuite&) = delete;

    void
    collect(const testlog&);

    void
    start_timing();

    void
    stop_timing();

    double
    get_duration() const;

    void
    set_keep_running(bool keep_running);

    bool
    get_keep_running() const;

    bool
    is_test_run(const std::string& class_name,
                const std::string& test_name) const;

    bool keep_running_;
    bool verbose_;
    bool failure_stop_;
    long n_tests_;
    long n_successes_;
    long n_failures_;
    long n_errors_;
    long n_skipped_;
    std::vector<testlog> testlogs_;
    std::string name_filter_;
    std::string test_name_;
    std::chrono::high_resolution_clock::time_point start_;
    std::chrono::high_resolution_clock::time_point end_;
};
/**
 * @brief A placeholder for the absent of a test context
 */
class nocontext {};
/**
 * @brief The class from which to derive when implementing a test case. The
 *  test context is optional
 */
template<typename TestContext=nocontext>
class testcase {
public:
    /**
     * @brief This is called before each test run. By default, it does not do
     *  anything
     */
    virtual void
    set_up()
    {}
    /**
     * @brief This is called after each test run. By default, it does not do
     *  anything
     */
    virtual void
    tear_down()
    {}
    /**
     * @brief Copy constructor. Deleted
     * @param other An instance of testcase
     */
    testcase(const testcase& other) = delete;
    /**
     * @brief Copy assignment operator. Deleted
     * @param other An instance of testcase
     * @returns An testcase instance
     */
    testcase&
    operator=(const testcase& other) = delete;
    /**
     * @brief Move constructor. Deleted
     * @param other An instance of testcase
     */
    testcase(testcase&& other) = delete;
    /**
     * @brief Move assignment operator. Deleted
     * @param other An instance of testcase
     * @returns An testcase instance
     */
    testcase&
    operator=(testcase&& other) = delete;

protected:
    /**
     * @brief Constructor
     */
    testcase()
        : context_(nullptr)
    {}
    /**
     * @brief Destructor
     */
    virtual ~testcase()
    {}
    /**
     * @brief Returns a pointer to the optional test context
     * @returns A pointer to the test context, nullptr if not defined
     */
    TestContext*
    get_test_context() const
    {
        return context_;
    }
    /**
     * @brief Returns the maximum displayed string length of a single object
     *  that is passed to any of the assertions
     * @returns The maximum displayed string length
     */
    virtual int
    max_displayed_string_length() const
    {
        return 100;
    }
    /**
     * @brief Returns the maximum displayed precision of numerical values that
     *  are passed to any of the assertions
     * @returns The maximum displayed value precision
     */
    virtual int
    max_displayed_value_precision() const
    {
        return 10;
    }
    /**
     * @brief Throws exception testfailure when called
     * @param message The exception message
     */
    virtual void
    fail(const std::string& message) const
    {
        throw testfailure(message);
    }
    /**
     * @brief Throws exception testfailure when called
     * @param assertion The name of the assertion
     * @param default_msg The default exception message
     * @param args An arbitrary number of arguments that are concatenated to a
     *  single string and are appended to the default message
     */
    template<typename... Args>
    void
    fail(const std::string& assertion,
         const std::string& default_msg,
         const Args&... args) const
    {
        fail(join(default_msg, " (", assertion, ") ", args...));
    }
    /**
     * @brief Asserts that a value is true
     * @param value A value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename... Args>
    void
    assert_true(const T& value,
                const Args&... message) const
    {
        if (!value) {
            const std::string msg = "false is not true";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is false
     * @param value A value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename... Args>
    void
    assert_false(const T& value,
                 const Args&... message) const
    {
        if (value) {
            const std::string msg = "true is not false";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two values are equal
     * @param expected The expected value
     * @param actual The actual value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename... Args>
    void
    assert_equal(const T& expected,
                 const U& actual,
                 const Args&... message) const
    {
        if (!(expected == actual)) {
            const std::string msg = _(expected) + " not equal to " + _(actual);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two values are not equal
     * @param first A value
     * @param second Another value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename... Args>
    void
    assert_not_equal(const T& first,
                     const U& second,
                     const Args&... message) const
    {
        if (first == second) {
            const std::string msg = _(first) + " equal to " + _(second);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two values are approximately equal up to some
     *  epsilon. The assertions succeeds if |a - b| < epsilon
     * @param expected The expected value
     * @param actual The actual value
     * @param eps The epsilon
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename V,
             typename... Args>
    void
    assert_approx_equal(const T& expected,
                        const U& actual,
                        const V& eps,
                        const Args&... message) const
    {
        if (!is_approx_equal(expected, actual, eps)) {
            const std::string msg = _(expected) + " not approx. equal to " + _(actual) + " with eps=" + _(eps);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two values are not approximately equal up to some
     *  epsilon. The assertions succeeds if |a - b| < epsilon is false
     * @param first A value
     * @param second Another value
     * @param eps The epsilon
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename V,
             typename... Args>
    void
    assert_approx_not_equal(const T& first,
                            const U& second,
                            const V& eps,
                            const Args&... message) const
    {
        if (is_approx_equal(first, second, eps)) {
            const std::string msg = _(first) + " approx. equal to " + _(second) + " with eps=" + _(eps);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that the first value is greater than the second
     * @param first A value
     * @param second Another value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename... Args>
    void
    assert_greater(const T& first,
                   const U& second,
                   const Args&... message) const
    {
        if (!(first > second)) {
            const std::string msg = _(first) + " not greater than " + _(second);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that the first value is greater than or equal to the second
     * @param first A value
     * @param second Another value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename... Args>
    void
    assert_greater_equal(const T& first,
                         const U& second,
                         const Args&... message) const
    {
        if (!(first >= second)) {
            const std::string msg = _(first) + " not greater than or equal to " + _(second);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that the first value is smaller than the second
     * @param first A value
     * @param second Another value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename... Args>
    void
    assert_smaller(const T& first,
                   const U& second,
                   const Args&... message) const
    {
        if (!(first < second)) {
            const std::string msg = _(first) + " not smaller than " + _(second);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that the first value is smaller than or equal to the second
     * @param first A value
     * @param second Another value
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename... Args>
    void
    assert_smaller_equal(const T& first,
                         const U& second,
                         const Args&... message) const
    {
        if (!(first <= second)) {
            const std::string msg = _(first) + " not smaller than or equal to " + _(second);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is in a given range with excluding bounds
     * @param value A value
     * @param lower The lower bound
     * @param upper The upper bound
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename V,
             typename... Args>
    void
    assert_in_range(const T& value,
                    const U& lower,
                    const V& upper,
                    const Args&... message) const
    {
        if (!is_in_range(value, lower, upper)) {
            const std::string msg = _(value) + " not in range (" + _(lower) + ", " + _(upper) + ")";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is not in a given range with excluding bounds
     * @param value A value
     * @param lower The lower bound
     * @param upper The upper bound
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename U,
             typename V,
             typename... Args>
    void
    assert_not_in_range(const T& value,
                        const U& lower,
                        const V& upper,
                        const Args&... message) const
    {
        if (is_in_range(value, lower, upper)) {
            const std::string msg = _(value) + " in range (" + _(lower) + ", " + _(upper) + ")";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is in a container
     * @param value A value
     * @param container A container
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename Container,
             typename... Args>
    void
    assert_in_container(const T& value,
                        const Container& container,
                        const Args&... message) const
    {
        if (!is_contained(value, container)) {
            const std::string msg = _(value) + " not in container";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is not in a container
     * @param value A value
     * @param container A container
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename Container,
             typename... Args>
    void
    assert_not_in_container(const T& value,
                            const Container& container,
                            const Args&... message) const
    {
        if (is_contained(value, container)) {
            const std::string msg = _(value) + " in container";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is approx. in a container up to some epsilon.
     *  The assertion succeeds if |a - b| < epsilon for at least one element
     * @param value A value
     * @param container A container
     * @param eps The epsilon
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename Container,
             typename U,
             typename... Args>
    void
    assert_approx_in_container(const T& value,
                               const Container& container,
                               const U& eps,
                               const Args&... message) const
    {
        if (!is_approx_contained(value, container, eps)) {
            const std::string msg = _(value) + " not approx. in container with eps=" + _(eps);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a value is not approx. in a container up to some
     *  epsilon. The assertion succeeds if |a - b| < epsilon is false for all elements
     * @param value A value
     * @param container A container
     * @param eps The epsilon
     * @param message If given, is appended to the default fail message
     */
    template<typename T,
             typename Container,
             typename U,
             typename... Args>
    void
    assert_approx_not_in_container(const T& value,
                                   const Container& container,
                                   const U& eps,
                                   const Args&... message) const
    {
        if (is_approx_contained(value, container, eps)) {
            const std::string msg = _(value) + " approx. in container with eps=" + _(eps);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two containers are equal
     * @param expected The expected container
     * @param actual The actual container
     * @param message If given, is appended to the default fail message
     */
    template<typename Container1,
             typename Container2,
             typename... Args>
    void
    assert_equal_containers(const Container1& expected,
                            const Container2& actual,
                            const Args&... message) const
    {
        if (!is_containers_equal(expected, actual)) {
            const std::string msg = "containers are not equal";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two containers are not equal
     * @param first A container
     * @param second Another container
     * @param message If given, is appended to the default fail message
     */
    template<typename Container1,
             typename Container2,
             typename... Args>
    void
    assert_not_equal_containers(const Container1& first,
                                const Container2& second,
                                const Args&... message) const
    {
        if (is_containers_equal(first, second)) {
            const std::string msg = "containers are equal";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two containers are approx. equal up to some epsilon.
     *  The assertion succeeds if |a - b| < epsilon for all pairs of elements
     * @param expected The expected container
     * @param actual The actual container
     * @param eps The epsilon
     * @param message If given, is appended to the default fail message
     */
    template<typename Container1,
             typename Container2,
             typename V,
             typename... Args>
    void
    assert_approx_equal_containers(const Container1& expected,
                                   const Container2& actual,
                                   const V& eps,
                                   const Args&... message) const
    {
        if (!is_containers_approx_equal(expected, actual, eps)) {
            const std::string msg = "containers are not approx. equal with eps=" + _(eps);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that two containers are not approx. equal up to some
     *  epsilon. The assertion succeeds if |a - b| < epsilon is false for at least one
     *  pair of elements
     * @param expected The expected container
     * @param actual The actual container
     * @param eps The epsilon
     * @param message If given, is appended to the default fail message
     */
    template<typename Container1,
             typename Container2,
             typename V,
             typename... Args>
    void
    assert_approx_not_equal_containers(const Container1& expected,
                                       const Container2& actual,
                                       const V& eps,
                                       const Args&... message) const
    {
        if (is_containers_approx_equal(expected, actual, eps)) {
            const std::string msg = "containers are approx. equal with eps=" + _(eps);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that all container elements match a given condition
     * @param container A container
     * @param condition A condition returning a boolean that can be applied to
     *  each container element
     * @param message If given, is appended to the default fail message
     */
    template<typename Container,
             typename Functor,
             typename... Args>
    void
    assert_all_of(const Container& container,
                  Functor condition,
                  const Args&... message) const
    {
        if (!std::all_of(std::begin(container), std::end(container), condition)) {
            const std::string msg = "Not all elements match the condition";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that not all container elements match a given condition
     * @param container A container
     * @param condition A condition returning a boolean that can be applied to
     *  each container element
     * @param message If given, is appended to the default fail message
     */
    template<typename Container,
             typename Functor,
             typename... Args>
    void
    assert_not_all_of(const Container& container,
                      Functor condition,
                      const Args&... message) const
    {
        if (std::all_of(std::begin(container), std::end(container), condition)) {
            const std::string msg = "All elements match the condition";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that any container element matches a given condition
     * @param container A container
     * @param condition A condition returning a boolean that can be applied to
     *  each container element
     * @param message If given, is appended to the default fail message
     */
    template<typename Container,
             typename Functor,
             typename... Args>
    void
    assert_any_of(const Container& container,
                  Functor condition,
                  const Args&... message) const
    {
        if (!std::any_of(std::begin(container), std::end(container), condition)) {
            const std::string msg = "No element matches the condition";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that none of the container elements match a given condition
     * @param container A container
     * @param condition A condition returning a boolean that can be applied to
     *  each container element
     * @param message If given, is appended to the default fail message
     */
    template<typename Container,
             typename Functor,
             typename... Args>
    void
    assert_none_of(const Container& container,
                   Functor condition,
                   const Args&... message) const
    {
        if (!std::none_of(std::begin(container), std::end(container), condition)) {
            const std::string msg = "At least one element matches the condition";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a string matches a regular expression
     * @param string A string
     * @param regex The regular expression
     * @param message If given, is appended to the default fail message
     */
    template<typename... Args>
    void
    assert_regex_match(const std::string& string,
                       const std::string& regex,
                       const Args&... message) const
    {
        if (!is_regex_matched(string, regex)) {
            const std::string msg = _(string) + " does not match the pattern " + _(regex);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a string matches a regular expression
     * @param string A string
     * @param regex The regular expression
     * @param message If given, is appended to the default fail message
     */
    template<typename... Args>
    void
    assert_regex_match(const std::string& string,
                       const std::regex& regex,
                       const Args&... message) const
    {
        if (!is_regex_matched(string, regex)) {
            const std::string msg = _(string) + " does not match the given pattern";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a string does not match a regular expression
     * @param string A string
     * @param regex The regular expression
     * @param message If given, is appended to the default fail message
     */
    template<typename... Args>
    void
    assert_not_regex_match(const std::string& string,
                           const std::string& regex,
                           const Args&... message) const
    {
        if (is_regex_matched(string, regex)) {
            const std::string msg = _(string) + " matches the pattern " + _(regex);
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a string does not match a regular expression
     * @param string A string
     * @param regex The regular expression
     * @param message If given, is appended to the default fail message
     */
    template<typename... Args>
    void
    assert_not_regex_match(const std::string& string,
                           const std::regex& regex,
                           const Args&... message) const
    {
        if (is_regex_matched(string, regex)) {
            const std::string msg = _(string) + " matches the given pattern";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a functor throws Exception
     * @param functor The functor
     * @param message If given, is appended to the default fail message
     */
    template<typename Exception,
             typename Functor,
             typename... Args>
    void
    assert_throw(Functor functor,
                 const Args&... message) const
    {
        bool caught = false;
        try {
            functor();
        } catch (const Exception&) {
            caught = true;
        }
        if (!caught) {
            const std::string msg = join("The exception was not thrown: ", typeid(Exception).name());
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Asserts that a functor does not throw any exception
     * @param functor The functor
     * @param message If given, is appended to the default fail message
     */
    template<typename Functor,
             typename... Args>
    void
    assert_no_throw(Functor functor,
                    const Args&... message) const
    {
        try {
            functor();
        } catch (const std::exception& e) {
            const std::string msg = join("An exception was thrown: ", typeid(e).name(), ": ", e.what());
            fail(__func__, msg, message...);
        } catch (...) {
            const std::string msg = "An unknown exception was thrown";
            fail(__func__, msg, message...);
        }
    }
    /**
     * @brief Utility function: Checks if two values are equal up to some epsilon
     * @param first A value
     * @param second Another value
     * @param eps The epsilon
     * @returns Whether the values are equal up to some epsilon
     */
    template<typename T,
             typename U,
             typename V>
    static bool
    is_approx_equal(const T& first,
                    const U& second,
                    const V& eps)
    {
        V diff = 0;
        if (first>second)
            diff = static_cast<V>(first - second);
        else if (first<second)
            diff = static_cast<V>(second - first);
        return diff < eps;
    }
    /**
     * @brief Checks whether the elements in two ranges are approx. equal
     *  up to some epsilon
     * @param first1 An input iterator
     * @param last1 An input iterator
     * @param first2 An input iterator
     * @param eps The epsilon
     * @returns Whether the elements in the two ranges are approx. equal
     */
    template <typename InputIterator1,
              typename InputIterator2,
              typename T>
    static bool
    is_approx_equal(InputIterator1 first1,
                    InputIterator1 last1,
                    InputIterator2 first2,
                    const T& eps)
    {
      while (first1!=last1) {
          if (!is_approx_equal(*first1, *first2, eps))
              return false;
          ++first1; ++first2;
      }
      return true;
    }
    /**
     * @brief Utility function: Checks if a value is in a given range.
     *  The bounds are excluding
     * @param value A value
     * @param lower The lower bound
     * @param upper The upper bound
     * @returns Whether the value is in a given range
     */
    template<typename T,
             typename U,
             typename V>
    static bool
    is_in_range(const T& value,
                const U& lower,
                const V& upper)
    {
        return value>lower && value<upper;
    }
    /**
     * @brief Utility function: Checks if a value is in a container
     * @param value A value
     * @param container A container
     * @returns Whether the value is in the container
     */
    template<typename T,
             typename Container>
    static bool
    is_contained(const T& value,
                 const Container& container)
    {
        auto first = std::begin(container);
        auto last = std::end(container);
        return std::find(first, last, value) != last;
    }
    /**
     * @brief Utility function: Checks if a value is approx. in a container
     *  up to some epsilon
     * @param value A value
     * @param container A container
     * @param eps The epsilon
     * @returns Whether the value is approx. in the container
     */
    template<typename T,
             typename Container,
             typename U>
    static bool
    is_approx_contained(const T& value,
                        const Container& container,
                        const U& eps)
    {
        auto first = std::begin(container);
        auto last = std::end(container);
        while (first!=last) {
            if (is_approx_equal(*first, value, eps)) return true;
            ++first;
        }
        return false;
    }
    /**
     * @brief Utility function: Checks if two containers are equal
     * @param first A container
     * @param second Another container
     * @returns Whether the containers are equal
     */
    template<typename Container1,
             typename Container2>
    static bool
    is_containers_equal(const Container1& first,
                        const Container2& second)
    {
        const bool equal_first = std::equal(std::begin(first), std::end(first),
                                            std::begin(second));
        const bool equal_second = std::equal(std::begin(second), std::end(second),
                                             std::begin(first));
        return equal_first && equal_second;
    }
    /**
     * @brief Utility function: Checks if two containers are approx. equal
     *  up to some epsilon
     * @param first A container
     * @param second Another container
     * @param eps The epsilon
     * @returns Whether the two containers are approx. equal
     */
    template<typename Container1,
             typename Container2,
             typename V>
    static bool
    is_containers_approx_equal(const Container1& first,
                               const Container2& second,
                               const V& eps)
    {
        const bool equal_first = is_approx_equal(std::begin(first), std::end(first),
                                                 std::begin(second), eps);
        const bool equal_second = is_approx_equal(std::begin(second), std::end(second),
                                                  std::begin(first), eps);
        return equal_first && equal_second;
    }
    /**
     * @brief Utility function: Checks if a value matches a regular expression
     * @param value A value
     * @param regex A regular expression
     * @returns Whether a value matches the regular expression
     */
    static bool
    is_regex_matched(const std::string& value,
                     const std::string& regex)
    {
        return std::regex_match(value, std::regex(regex));
    }
    /**
     * @brief Utility function: Checks if a value matches a regular expression
     * @param value A value
     * @param regex A regular expression
     * @returns Whether a value matches the regular expression
     */
    static bool
    is_regex_matched(const std::string& value,
                     const std::regex& regex)
    {
        return std::regex_match(value, regex);
    }

private:
    template<typename ContextClass,
             typename TestCase>
    friend void
    testrun(ContextClass&,
            void (TestCase::*)(),
            const std::string&,
            const std::string&);

    void
    set_test_context(TestContext *context)
    {
        context_ = context;
    }

    template<typename T>
    std::string
    _(const T& value) const
    {
        std::ostringstream stream;
        stream.precision(max_displayed_value_precision());
        stream << value;
        std::string result(stream.str());
        if (int(result.size())>max_displayed_string_length()) {
            result = result.substr(0, max_displayed_string_length());
        }
        return std::move("'" + result + "'");
    }

    TestContext *context_;
};
/**
 * @brief Writes the test results as an XML to the given output stream
 * @param stream The output stream
 * @param results The test results
 */
void
write_xml(std::ostream& stream,
          const testresults& results);
/**
 * @brief Writes the test summary to the given output stream
 * @param stream The output stream
 * @param results The test results
 */
void
write_summary(std::ostream& stream,
              const testresults& results);
/**
 * @brief Writes the error info to the given output stream
 * @param stream The output stream
 * @param results The test results
 */
void
write_error_info(std::ostream& stream,
                 const testresults& results);
/**
 * @brief Writes a test start message to the given output stream
 * @param stream The output stream
 * @param log A test log
 * @param verbose Whether to use verbose output
 */
void
write_test_start_message(std::ostream& stream,
                         const testlog& log,
                         bool verbose);
/**
 * @brief Writes a test end message to the given output stream
 * @param stream The output stream
 * @param log A test log
 * @param verbose Whether to use verbose output
 */
void
write_test_end_message(std::ostream& stream,
                       const testlog& log,
                       bool verbose);
/**
 * @brief Writes a horizontal bar to the given output stream
 * @param stream The output stream
 * @param character The character to repeat
 * @param length The length of the bar
 */
void
write_horizontal_bar(std::ostream& stream,
                     char character,
                     int length=50);
/**
 * @brief Returns whether a given test is executed
 * @param test_name The full test name
 * @param exact_name An exact name to be checked for, ignored if empty
 * @param filter_name A filter for the beginning of the test name, ignored if empty
 * @returns Whether a given test is executed
 */
bool
is_test_executed(const std::string& test_name,
                 const std::string& exact_name,
                 const std::string& filter_name);
/**
 * @brief Casts a duration value to seconds
 * @param duration The duration value
 * @returns The duration in seconds
 */
double
duration_in_seconds(const std::chrono::duration<double>& duration);
/**
 * @brief The test runner that is called by the testrun function
 */
class testrunner {
public:
    /**
     * @brief Constructor
     * @param class_name The name of the test class
     * @param test_name The name of the current test method
     */
    testrunner(const std::string& class_name,
               const std::string& test_name);
    /**
     * @brief Destructor
     */
    virtual ~testrunner();
    /**
     * @brief Returns whether to execute the current test
     * @returns Whether to execute the current test
     */
    bool
    is_executed();
    /**
     * @brief Executes the test method
     * @param test An instance of a testcase class
     * @param method A test method belonging to the testcase class
     */
    template<typename TestCase>
    void
    execute(TestCase& test,
            void (TestCase::*method)())
    {
        try {
            (test.*method)();
            log_.status = teststatus::success;
            log_.message = "ok";
        } catch (const testfailure& e) {
            log_.status = teststatus::failure;
            log_.error_type = "testfailure";
            log_.message = e.what();
        } catch (const std::exception& e) {
            log_.status = teststatus::error;
            log_.error_type = typeid(e).name();
            log_.message = e.what();
        } catch (...) {
            log_.status = teststatus::error;
            log_.error_type = "Unknown exception";
            log_.message = "Unknown message";
        }
        write_test_end_message(std::cout, log_, suite_->verbose_);
    }

private:

    testrunner(const testrunner&) = delete;

    testrunner&
    operator=(const testrunner&) = delete;

    testlog log_;
    std::chrono::high_resolution_clock::time_point start_;
    testsuite *suite_;
    bool is_run_;
};
/**
 * @brief A test run (thread-safe)
 * @param method A pointer to the method to be run
 * @param class_name The name of the test class
 * @param test_name The name of the current test method
 */
template<typename TestCase>
void
testrun(void (TestCase::*method)(),
        const std::string& class_name,
        const std::string& test_name)
{
    testrunner runner(class_name, test_name);
    if (runner.is_executed()) {
        TestCase test;
        test.set_up();
        runner.execute(test, method);
        test.tear_down();
    }
}
/**
 * @brief A test run with a test context (thread-safe)
 * @param context The test context
 * @param method A pointer to the method to be run
 * @param class_name The name of the test class
 * @param test_name The name of the current test method
 */
template<typename TestContext,
         typename TestCase>
void
testrun(TestContext& context,
        void (TestCase::*method)(),
        const std::string& class_name,
        const std::string& test_name)
{
    testrunner runner(class_name, test_name);
    if (runner.is_executed()) {
        TestCase test;
        test.set_test_context(&context);
        test.set_up();
        runner.execute(test, method);
        test.tear_down();
    }
}
/**
 * @brief An argument parser to be used from the users main() function
 */
class argparser : public userargs {
public:
    /**
     * @brief Constructor
     * @param argc The number of arguments
     * @param argv The container of the arguments
     */
    argparser(int argc, char **argv);

private:

    void
    print_help(std::ostream& stream) const;

    void
    help_exit_failure(const std::string& message) const;
};
/**
 * @brief The exception class to indicate argument errors
 */
class argparser_error : public std::runtime_error {
public:
    /**
     * @brief Constructor
     * @param message The exception message
     */
    explicit argparser_error(const std::string& message);
};

} // unittest


// helper macros

/**
 * @brief A test run (thread-safe)
 * @param klass The test class
 * @param method The test method
 */
#define UNITTEST_RUN(klass, method) \
        unittest::testrun(&klass::method, #klass, #method);
/**
 * @brief A test run with a test context (thread-safe)
 * @param context The test context
 * @param klass The test class
 * @param method The test method
 */
#define UNITTEST_RUNCXT(context, klass, method) \
        unittest::testrun(context, &klass::method, #klass, #method);
/**
 * @brief Defines a string with info about the current file name
 *  and the current line number
 */
#define UNITTEST_SPOT \
        unittest::join(" @", __FILE__, ":", __LINE__, ". ")


// implementation details: misc.

inline std::tuple<int,int,int>
unittest::get_version_tuple()
{
    return std::make_tuple(2, 5, 0);
}

inline std::string
unittest::get_version_string()
{
    const auto vs = get_version_tuple();
    return join(std::get<0>(vs), ".", std::get<1>(vs), ".", std::get<2>(vs));
}

inline int
unittest::make_default_environment(int argc, char **argv)
{
    int exit_code;
    try {
        unittest::argparser args(argc, argv);
        auto suite = unittest::testsuite::instance();
        suite->set_verbose(args.verbose());
        suite->set_failure_stop(args.failure_stop());
        suite->set_name_filter(args.name_filter());
        suite->set_test_name(args.test_name());
        unittest::run_all_tests();
        const auto results = suite->get_results();
        unittest::write_error_info(std::cout, results);
        unittest::write_summary(std::cout, results);
        if (args.generate_xml()) {
            std::ofstream file(args.xml_filename());
            unittest::write_xml(file, results);
        }
        if (results.successful)
            exit_code = EXIT_SUCCESS;
        else
            exit_code = EXIT_FAILURE;
    } catch (const argparser_error& e) {
        std::cout << "ArgumentError: " << e.what();
        std::exit(EXIT_FAILURE);
    }
    return exit_code;
}

inline void
unittest::write_summary(std::ostream& stream,
                        const testresults& results)
{
    stream << "\n";
    write_horizontal_bar(stream, '-');
    stream << "\n";
    stream << "Ran " << results.n_tests << " tests in ";
    stream << results.duration << "s\n\n";
    if (results.n_tests==results.n_successes) {
        stream << "OK\n";
    } else {
        stream << "FAILED (";
        if (results.n_failures>0 && results.n_errors>0) {
            stream << "failures=" << results.n_failures;
            stream << ", errors=" << results.n_errors;
        } else if (results.n_failures>0) {
            stream << "failures=" << results.n_failures;
        } else if (results.n_errors>0) {
            stream << "errors=" << results.n_errors;
        }
        stream << ")\n";
    }
    stream << std::flush;
}

inline void
unittest::write_error_info(std::ostream& stream,
                           const testresults& results)
{
    if (!results.successful) {
        stream << "\n";
        for (auto log : results.testlogs) {
            const auto status = log.status;
            if (status==teststatus::failure || status==teststatus::error) {
                write_horizontal_bar(stream, '=');
                stream << "\n";
                std::string flag("FAIL");
                if (status==teststatus::error)
                    flag = "ERROR";
                stream << flag << ": " << log.class_name << "." << log.test_name;
                stream << "\n";
                write_horizontal_bar(stream, '-');
                stream << "\n";
                stream << log.error_type << ": " << log.message;
                stream << "\n\n";
            }
        }
        stream << std::flush;
    }
}

inline void
unittest::write_xml(std::ostream& stream,
                    const testresults& results)
{
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    stream << "\n";
    stream << "<testsuite name=\"libunittest";
    stream << "\" tests=\"" << results.n_tests;
    stream << "\" skipped=\"" << results.n_skipped;
    stream << "\" errors=\"" << results.n_errors;
    stream << "\" failures=\"" << results.n_failures;
    stream << "\" time=\"" << results.duration << "\">";
    stream << "\n";
    for (auto log : results.testlogs) {
        stream << "\t<testcase classname=\"" << log.class_name;
        stream << "\" name=\"" << log.test_name;
        stream << "\" time=\"" << log.duration << "\"";
        if (log.successful) {
            stream << "/>";
            stream << "\n";
        } else {
            stream << ">" << std::endl;
            if (log.status==teststatus::failure)
                stream << "\t\t<failure ";
            else
                stream << "\t\t<error ";
            stream << "type=\"" << log.error_type;
            stream << "\" message=\"" << log.message << "\"/>";
            stream << "\n";
            stream << "\t</testcase>";
            stream << "\n";
        }
    }
    stream << "</testsuite>";
    stream << "\n";
    stream << std::flush;
}

inline bool
unittest::is_test_executed(const std::string& test_name,
                           const std::string& exact_name,
                           const std::string& filter_name)
{
    if (exact_name!="") {
        if (exact_name==test_name)
            return true;
        else
            return false;
    }

    if (filter_name!="") {
        if (test_name.substr(0, filter_name.size())==filter_name)
            return true;
        else
            return false;
    }

    return true;
}

inline void
unittest::write_test_start_message(std::ostream& stream,
                                   const testlog& log,
                                   bool verbose)
{
    if (verbose) {
        stream << log.class_name << "." << log.test_name << " ... ";
        stream << std::flush;
    }
}

inline void
unittest::write_test_end_message(std::ostream& stream,
                                 const testlog& log,
                                 bool verbose)
{
    if (verbose) {
        switch (log.status) {
        case teststatus::success: stream << "ok"; break;
        case teststatus::failure: stream << "FAIL"; break;
        case teststatus::error: stream << "ERROR"; break;
        case teststatus::skipped: break;
        }
        stream << "\n";
    } else {
        switch (log.status) {
        case teststatus::success: stream << "."; break;
        case teststatus::failure: stream << "F"; break;
        case teststatus::error: stream << "E"; break;
        case teststatus::skipped: break;
        }
    }
    stream << std::flush;
}

inline double
unittest::duration_in_seconds(const std::chrono::duration<double>& duration)
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}

inline void
unittest::write_horizontal_bar(std::ostream& stream,
                               char character,
                               int length)
{
    std::string bar(length, character);
    stream << bar << std::flush;
}

inline void
unittest::write_to_stream(std::ostream&)
{}


// implementation details: testfailure

inline
unittest::testfailure::testfailure(const std::string& message)
    : std::runtime_error(message)
{}


// implementation details: testlog

inline
unittest::testlog::testlog()
    : class_name(""), test_name(""), successful(true),
      status(teststatus::skipped), error_type(), message(""), duration(0)
{}


// implementation details: testresults

inline
unittest::testresults::testresults()
    : successful(true), n_tests(0), n_successes(0), n_failures(0),
      n_errors(0), n_skipped(0),  duration(0), testlogs(0)
{}


// implementation details: testsuite

inline unittest::testsuite*
unittest::testsuite::instance()
{
    static std::unique_ptr<testsuite> instance_ = nullptr;
    if (instance_==nullptr) {
        static std::mutex instance_mutex_;
        std::lock_guard<std::mutex> lock(instance_mutex_);
        if (instance_==nullptr)
            instance_.reset(new testsuite());
    }
    return instance_.get();
}

inline
unittest::testsuite::testsuite()
    : keep_running_(true), verbose_(false), failure_stop_(false),
      n_tests_(0), n_successes_(0), n_failures_(0),
      n_errors_(0), n_skipped_(0), testlogs_(0), name_filter_(), test_name_(),
      start_(std::chrono::high_resolution_clock::time_point::min()),
      end_(std::chrono::high_resolution_clock::time_point::min())
{}

inline void
unittest::testsuite::set_verbose(bool verbose)
{
    static std::mutex set_verbose_mutex_;
    std::lock_guard<std::mutex> lock(set_verbose_mutex_);
    verbose_ = verbose;
}

inline void
unittest::testsuite::set_failure_stop(bool failure_stop)
{
    static std::mutex set_failure_stop_mutex_;
    std::lock_guard<std::mutex> lock(set_failure_stop_mutex_);
    failure_stop_ = failure_stop;
}

inline void
unittest::testsuite::set_name_filter(const std::string& name_filter)
{
    static std::mutex set_name_filter_mutex_;
    std::lock_guard<std::mutex> lock(set_name_filter_mutex_);
    name_filter_ = name_filter;
}

inline void
unittest::testsuite::set_test_name(const std::string& test_name)
{
    static std::mutex set_test_name_mutex_;
    std::lock_guard<std::mutex> lock(set_test_name_mutex_);
    test_name_ = test_name;
}

inline void
unittest::testsuite::start_timing()
{
    static std::mutex start_timing_mutex_;
    std::lock_guard<std::mutex> lock(start_timing_mutex_);
    if (start_==std::chrono::high_resolution_clock::time_point::min())
        start_ = std::chrono::high_resolution_clock::now();
}

inline void
unittest::testsuite::stop_timing()
{
    static std::mutex stop_timing_mutex_;
    std::lock_guard<std::mutex> lock(stop_timing_mutex_);
    end_ = std::chrono::high_resolution_clock::now();
}

inline double
unittest::testsuite::get_duration() const
{
    return duration_in_seconds(end_ - start_);
}

inline unittest::testresults
unittest::testsuite::get_results() const
{
    testresults results;
    results.successful = n_tests_==n_successes_;
    results.n_tests = n_tests_;
    results.n_successes = n_successes_;
    results.n_failures = n_failures_;
    results.n_errors = n_errors_;
    results.n_skipped = n_skipped_;
    results.duration = get_duration();
    results.testlogs = testlogs_;
    return std::move(results);
}

inline bool
unittest::testsuite::get_keep_running() const
{
    return keep_running_;
}

inline void
unittest::testsuite::set_keep_running(bool keep_running)
{
    static std::mutex set_keep_running_mutex_;
    std::lock_guard<std::mutex> lock(set_keep_running_mutex_);
    keep_running_ = keep_running;
}

inline bool
unittest::testsuite::is_test_run(const std::string& class_name,
                                 const std::string& test_name) const
{
    const std::string full_name = class_name + "." + test_name;
    return is_test_executed(full_name, test_name_, name_filter_);
}

inline void
unittest::testsuite::collect(const testlog& log)
{
    static std::mutex collect_mutex_;
    std::lock_guard<std::mutex> lock(collect_mutex_);
    switch (log.status) {
    case teststatus::success: ++n_successes_; break;
    case teststatus::failure: ++n_failures_; break;
    case teststatus::error: ++n_errors_; break;
    case teststatus::skipped: ++n_skipped_; break;
    }
    if (log.status!=teststatus::skipped) {
        ++n_tests_;
        testlogs_.push_back(std::move(log));
    }
}


// implementation details: userargs

inline
unittest::userargs::userargs()
    : verbose_(false), failure_stop_(false), generate_xml_(false),
      name_filter_(""), test_name_(""), xml_filename_("libunittest.xml")
{}

inline
unittest::userargs::~userargs()
{}

inline bool
unittest::userargs::verbose() const
{
    return verbose_;
}

inline void
unittest::userargs::verbose(bool value)
{
    verbose_ = value;
}

inline bool
unittest::userargs::failure_stop() const
{
    return failure_stop_;
}

inline void
unittest::userargs::failure_stop(bool value)
{
    failure_stop_ = value;
}

inline bool
unittest::userargs::generate_xml() const
{
    return generate_xml_;
}

inline void
unittest::userargs::generate_xml(bool value)
{
    generate_xml_ = value;
}

inline std::string
unittest::userargs::name_filter() const
{
    return name_filter_;
}

inline void
unittest::userargs::name_filter(const std::string& value)
{
    name_filter_ = value;
}

inline std::string
unittest::userargs::test_name() const
{
    return test_name_;
}

inline void
unittest::userargs::test_name(const std::string& value)
{
    test_name_ = value;
}

inline std::string
unittest::userargs::xml_filename() const
{
    return xml_filename_;
}

inline void
unittest::userargs::xml_filename(const std::string& value)
{
    xml_filename_ = value;
}


// implementation details: argparser

inline
unittest::argparser::argparser(int argc, char **argv)
    : userargs()
{
    for (int i=1; i<argc; ++i) {
        const std::string value(argv[i]);
        if (value=="-h") {
            print_help(std::cout);
            std::exit(EXIT_SUCCESS);
        } else if (value=="-v") {
            verbose(true);
        } else if (value=="-s") {
            failure_stop(true);
        } else if (value=="-x") {
            generate_xml(true);
        } else if (value=="-sv" || value=="-vs") {
            verbose(true);
            failure_stop(true);
        } else if (value=="-vx" || value=="-xv") {
            verbose(true);
            generate_xml(true);
        } else if (value=="-sx" || value=="-xs") {
            failure_stop(true);
            generate_xml(true);
        } else if (value=="-vsx" || value=="-svx" || value=="-xvs" ||
                   value=="-xsv" || value=="-vxs" || value=="-sxv") {
            verbose(true);
            failure_stop(true);
            generate_xml(true);
        } else if (value=="-f") {
            if (++i<argc) {
                name_filter(argv[i]);
            } else {
                help_exit_failure("Option '-f' needs a filter string");
            }
        } else if (value=="-t") {
            if (++i<argc) {
                test_name(argv[i]);
            } else {
                help_exit_failure("Option '-t' needs a test name");
            }
        } else if (value=="-o") {
            if (++i<argc) {
                xml_filename(argv[i]);
            } else {
                help_exit_failure("Option '-o' needs an XML file name");
            }
        } else {
            help_exit_failure(join("Unknown argument '", value, "'"));
        }
    }
}

inline void
unittest::argparser::help_exit_failure(const std::string& message) const
{
    std::ostringstream stream;
    print_help(stream);
    throw argparser_error(join(message, "\n\n", stream.str()));
}

inline void
unittest::argparser::print_help(std::ostream& stream) const
{
    stream << "This is your testing application using libunittest-";
    stream << get_version_string() << "\n\n";
    stream << "Available options are:\n";
    stream << "-h          Displays this help message\n";
    stream << "-v          Sets verbose output for running tests\n";
    stream << "-s          Stops running tests after the first test fails\n";
    stream << "-x          Enables the generation of the XML output\n";
    stream << "-f filter   Specifies a filter applied to the beginning of the test names\n";
    stream << "-t test     Specifies a certain test to run superseding the name filter\n";
    stream << "-o xmlfile  Specifies the XML output file name (default: libunittest.xml)\n";
    stream << std::flush;
}


// implementation details: argparser_error

inline
unittest::argparser_error::argparser_error(const std::string& message)
    : std::runtime_error(message)
{}


// implementation details: testrunner

inline
unittest::testrunner::testrunner(const std::string& class_name,
                                 const std::string& test_name)
    : log_(),
	  start_(),
	  suite_(testsuite::instance()),
      is_run_(true)
{
    if (!suite_->get_keep_running())
        is_run_ = false;
    else if (!suite_->is_test_run(class_name, test_name))
        is_run_ = false;

    if (is_run_) {
        suite_->start_timing();
        start_ = std::chrono::high_resolution_clock::now();
        log_.class_name = class_name;
        log_.test_name = test_name;
        write_test_start_message(std::cout, log_, suite_->verbose_);
    }
}

inline
unittest::testrunner::~testrunner()
{
    if (is_run_) {
        log_.successful = log_.status==teststatus::success;
        if (!log_.successful && suite_->failure_stop_)
            suite_->set_keep_running(false);
        const auto end = std::chrono::high_resolution_clock::now();
        suite_->stop_timing();
        log_.duration = duration_in_seconds(end - start_);
    }
    suite_->collect(log_);
}

inline bool
unittest::testrunner::is_executed()
{
    return is_run_;
}
