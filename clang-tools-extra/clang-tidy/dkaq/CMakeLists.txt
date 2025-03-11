add_clang_library(dkaqPlugin STATIC
  UnusedVarDiagnostic.cpp
  VirtualDestructorCheck.cpp
  dkaqmodule.cpp
  R-1-1-7.cpp
  R-1-1-8.cpp
  R-1-1-10.cpp

  LINK_LIBS
  clangTidy
  ${ALL_CLANG_TIDY_CHECKS}

  DEPENDS
  omp_gen
  ClangDriverOptions
  )

clang_target_link_libraries(dkaqPlugin
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSema
  clangTooling
  )
