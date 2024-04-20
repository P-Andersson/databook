add_executable(databook_tests)

target_sources(databook_tests
               PRIVATE
                  src/notebook/test_notebook.cpp
)

find_package(Catch2 REQUIRED)

target_link_libraries(databook_tests PRIVATE databook Catch2::Catch2WithMain)

add_test(NAME databook_tests COMMAND databook_tests)
