#include <gtest_utils.h>

#include <scl/utility/preprocessor/threads.h>

#ifndef SCL_HAS_THREADS
#error "SCL_HAS_THREADS must always be defined, so that #if catches a misspelling"
#endif

// The hosted library of MSVC and libstdc++ on glibc always supports threads.
#if defined(_MSVC_STL_VERSION) || (defined(__GLIBCXX__) && defined(__GLIBC__))
/**
 * @test Verify that a hosted library known to support threads reports them.
 */
TEST(PreprocessorThreads, AHostedLibraryReportsThreads) { STATIC_EXPECT_EQ(SCL_HAS_THREADS, 1); }
#endif

#if SCL_HAS_THREADS
#include <mutex>
#include <thread>

/**
 * @test Verify that a library reporting threads starts a thread that takes a lock.
 */
TEST(PreprocessorThreads, AThreadedLibraryRunsAThreadUnderALock)
{
    ::std::mutex mutex;
    int runs = 0;
    ::std::thread{[&mutex, &runs] {
        ::std::lock_guard const lock{mutex};
        ++runs;
    }}.join();

    EXPECT_EQ(runs, 1);
}
#endif
