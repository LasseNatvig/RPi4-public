# Quadratic Equation Solver - Example code for TDT4258 - 2026 edition

<!-- Initially written by Cline and MistralMedium -->

Demonstration of solving quadratic equations with unit testing using the Unity framework.

The example uses the unit-testing framework called [Unity](https://www.throwtheswitch.org/unity)

## Structure

- `src/` - Production code (quadratic equation solver)
- `tests/` - Unit tests for quadratic solver
  - `tests/test_quadratic.c` - Unit tests for quadratic solver functionality
  - `tests/all_tests.c` - Test runner that runs all unit tests
- `unity/` - Unity test framework
- `.cline/skills/c-testing/` - Cline skill for C testing
- `.cline/skills/coding-style/` - Cline skill for enforcing C coding style
- `.cline/skills/markdown-lint/` - Cline skill for fixing markdown-lint errors

## Build & Run

### Building the Project

```bash
nmake all              # Build all targets (debug.exe, release.exe, all test executables)
nmake -nologo all      # Avoids the MS logo text
nmake debug.exe        # Build debug version of main program
nmake release.exe      # Build release version of main program
nmake test_all.exe     # Build all tests
nmake clean            # Clean all build files
```

### Running Tests

```bash
nmake run-tests               # Run all tests
```

Or execute the test binary directly:

```bash
./test_all.exe        # Run all tests
```

## Quadratic Equation

The solver implements: ax^2 + bx + c = 0
Solutions: x = [-b +/- sqrt(b^2 - 4ac)] / (2a)
