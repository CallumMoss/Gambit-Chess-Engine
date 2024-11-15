add_test([=[PERFT.perft_suite]=]  C:/Users/callu/Desktop/Gambit/build/Tests.exe [==[--gtest_filter=PERFT.perft_suite]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[PERFT.perft_suite]=]  PROPERTIES WORKING_DIRECTORY C:/Users/callu/Desktop/Gambit/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  Tests_TESTS PERFT.perft_suite)
