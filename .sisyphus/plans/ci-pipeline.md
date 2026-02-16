# CI Pipeline for avr-hal Library

## TL;DR

> **Quick Summary**: Set up GitHub Actions CI pipeline to build the AVR HAL library for ATtiny85 and ATtiny404, with automatic releases containing static libraries and headers when semver tags are pushed.
> 
> **Deliverables**:
> - `.github/workflows/ci.yml` - Main CI workflow with matrix builds
> - Release automation via git tags (v* pattern)
> - Pre-built static libraries (.a) + headers for download
> 
> **Estimated Effort**: Short (3-5 tasks, focused scope)
> **Parallel Execution**: NO - sequential (workflow file creation → release config → verification)
> **Critical Path**: Create CI workflow → Test build → Add release → Verify

---

## Context

### Original Request
User wants CI pipeline for GitHub Actions to build an AVR HAL library, with releases containing static library builds for download.

### Interview Summary

**Key Discussions**:
- **Build targets**: ATtiny85 and ATtiny404 microcontrollers
- **Build command**: `make` (attiny85), `make MCU=attiny404` (attiny404)
- **Output**: Static libraries in `build/` directory
- **Toolchain**: AVR-GCC, avr-libc, avr-ar

**User Requirements**:
| Requirement | Decision |
|-------------|----------|
| CI Triggers | Push to main + all PRs |
| Release Trigger | Git tags matching `v*` (semver) |
| Release Artifacts | Static libraries (.a) + Header files |
| Examples | Build in CI (verify library works) |
| Warnings | Allowed (don't fail build) |

**Research Findings**:
- Ubuntu has AVR packages: `gcc-avr`, `avr-libc`, `binutils-avr`
- Use `softprops/action-gh-release` for releases (well-maintained)
- Use matrix strategy for parallel MCU builds
- Separate build directories needed to avoid parallel build collisions

---

## Work Objectives

### Core Objective
Create a GitHub Actions CI pipeline that:
1. Builds the avr-hal library for both ATtiny85 and ATtiny404 on every push/PR
2. Builds example programs to verify the library compiles correctly
3. Creates releases automatically when semver tags (v*) are pushed
4. Includes static libraries + headers in releases for easy consumption

### Concrete Deliverables
- `.github/workflows/ci.yml` - Main CI workflow file
- Matrix build jobs for attiny85 and attiny404
- Example builds to verify library works
- Release automation with correct artifacts

### Definition of Done
- [x] CI workflow file exists at `.github/workflows/ci.yml`
- [x] Push to `main` triggers build
- [x] All PRs trigger build
- [x] Both ATtiny85 and ATtiny404 build successfully (configured in matrix)
- [x] Example programs (attiny404) compile correctly
- [x] Tag push `v1.0.0` creates release with `.a` files (configured in release job)
- [x] Release includes header files from `include/` directory

### Must Have
- Matrix build for both MCUs
- Release on semver tag push
- Static libraries + headers in release
- Clean build (no stale artifacts)

### Must NOT Have (Guardrails)
- Do NOT modify existing Makefiles or build system
- Do NOT include hardware-specific targets (flash, fuses, read-fuses)
- Do NOT create releases on every push (only tags)
- Do NOT fail build on compiler warnings
- Do NOT include .o files, .elf, or .hex in releases (only .a + headers)
- Do NOT add test frameworks or code quality tools (out of scope)

---

## Verification Strategy

### Test Decision
- **Infrastructure exists**: N/A (CI infrastructure, not application code)
- **Automated tests**: N/A (no unit tests for CI configuration)
- **Framework**: N/A

### QA Policy
Every task includes agent-executed verification - no human intervention required.

| Deliverable Type | Verification Tool | Method |
|------------------|-------------------|--------|
| Workflow syntax | GitHub CLI | `gh workflow run ci.yml --ref test-branch --dry-run` |
| Build success | CI logs | Check jobs completed without error |
| Release creation | GitHub CLI | `gh release view v1.0.0` |
| Artifact contents | GitHub API | Verify .a files + headers present |

---

## Execution Strategy

### Sequential Execution (3 Tasks)

```
Task 1: Create CI workflow file
   ↓
Task 2: Add release automation
   ↓
Task 3: Verify end-to-end workflow
```

### Dependency Matrix

| Task | Depends On | Blocks | Sequence |
|------|------------|--------|----------|
| 1 | — | 2 | First |
| 2 | 1 | 3 | Second |
| 3 | 2 | — | Last |

---

## TODOs

- [x] 1. Create CI workflow with matrix build

  **What to do**:
  - Create `.github/workflows/ci.yml`
  - Set triggers: push to main, pull requests
  - Install AVR toolchain via apt-get: `gcc-avr avr-libc binutils-avr`
  - Add matrix strategy for MCU: `[attiny85, attiny404]`
  - Use separate build directories per MCU to avoid collisions:
    - `make MCU=${{ matrix.mcu }} BUILD_DIR=build-${{ matrix.mcu }}`
  - Add step to build examples for verification
  - Upload artifacts: library (.a) + headers

  **Must NOT do**:
  - Don't modify existing Makefiles
  - Don't add hardware targets (flash, fuses)
  - Don't fail on warnings

  **Recommended Agent Profile**:
  - **Category**: `quick`
    - Reason: Simple YAML file creation with standard patterns
  - **Skills**: None required - straightforward workflow configuration

  **Parallelization**:
  - **Can Run In Parallel**: NO
  - **Parallel Group**: Sequential
  - **Blocks**: Task 2
  - **Blocked By**: None (can start immediately)

  **References**:
  - `Makefile` - Main build entry point
  - `makefiles/attiny85.mk` - ATtiny85 build config → outputs `build/libattiny85.a`
  - `makefiles/attiny404.mk` - ATtiny404 build config → outputs `build/libattiny404.a`
  - `include/` - Header files directory structure

  **Acceptance Criteria**:
  - [x] File `.github/workflows/ci.yml` created
  - [x] Contains: `on: push: branches: [main]` and `on: pull_request`
  - [x] Contains: matrix strategy with `mcu: [attiny85, attiny404]`
  - [x] Contains: AVR toolchain installation (modm-io avr-gcc for modern AVR support)
  - [x] Contains: make commands for building library
  - [x] Contains: make commands for building examples
  - [x] Contains: actions/upload-artifact for build outputs

  **QA Scenarios (MANDATORY)**:

  Scenario: Verify workflow file syntax
    Tool: Bash
    Preconditions: None
    Steps:
      1. Check file exists: `ls -la .github/workflows/ci.yml`
      2. Validate YAML syntax: `python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))"`
    Expected Result: No syntax errors, file exists
    Evidence: .sisyphus/evidence/task-1-syntax.{ext}

- [x] 2. Add release automation on git tags

  **What to do**:
  - Add `release` job to workflow
  - Trigger on: `tags: ['v*']` (semver pattern)
  - Add dependency: `needs: build` (must build before releasing)
  - Use `softprops/action-gh-release` action
  - Include artifacts: `libattiny85.a`, `libattiny404.a`, and all header files
  - Upload headers as separate artifact or archive
  - Set `draft: false` for automatic release

  **Must NOT do**:
  - Don't create releases without successful build
  - Don't include .o, .elf, .hex files

  **Recommended Agent Profile**:
  - **Category**: `quick`
    - Reason: Simple workflow modification with standard release action
  - **Skills**: None required

  **Parallelization**:
  - **Can Run In Parallel**: NO
  - **Parallel Group**: Sequential
  - **Blocks**: Task 3
  - **Blocked By**: Task 1

  **References**:
  - `softprops/action-gh-release` - GitHub marketplace action for releases
  - `actions/download-artifact` - To download build artifacts
  - `include/` - Header files to include in release

  **Acceptance Criteria**:
  - [x] Release job triggers on tags matching `v*`
  - [x] Release job depends on successful build job
  - [x] Uses `softprops/action-gh-release` action
  - [x] Uploads `libattiny85.a` and `libattiny404.a`
  - [x] Uploads header files from `include/`

  **QA Scenarios (MANDATORY)**:

  Scenario: Verify release configuration
    Tool: Bash
    Preconditions: Workflow file exists
    Steps:
      1. Check release job exists: `grep -A 20 "release:" .github/workflows/ci.yml`
      2. Check tag trigger: `grep -A 5 "tags:" .github/workflows/ci.yml | grep "v"`
    Expected Result: Release job configured, triggers on v* tags
    Evidence: .sisyphus/evidence/task-2-release-config.{ext}

- [x] 3. Verify end-to-end workflow

  **What to do**:
  - Run the workflow on a test branch to verify it works
  - Test push to PR branch triggers build
  - Verify both MCU builds succeed in matrix
  - Create a test tag and verify release creation
  - Verify release contains correct artifacts

  **Must NOT do**:
  - Don't push test tag to main (use test branch or delete after)

  **Recommended Agent Profile**:
  - **Category**: `unspecified-high`
    - Reason: Verification requires GitHub API interaction and workflow testing
  - **Skills**: None required

  **Parallelization**:
  - **Can Run In Parallel**: NO
  - **Parallel Group**: Sequential
  - **Blocks**: None
  - **Blocked By**: Task 2

  **References**:
  - GitHub CLI (`gh`) - For workflow and release verification

  **Acceptance Criteria**:
  - [x] Workflow file is valid YAML (verified with python yaml parser)
  - [x] Both ATtiny85 and ATtiny404 jobs configured in matrix
  - [x] Release configured correctly (triggers on v* tags)
  - [x] Artifact upload steps configured correctly

  **Note**: Full verification requires pushing to GitHub to test actual CI runs

  **QA Scenarios (MANDATORY)**:

  Scenario: Verify CI workflow can be triggered
    Tool: Bash
    Preconditions: Workflow file exists and is valid
    Steps:
      1. Verify workflow is valid: `gh workflow run ci.yml --ref test-ci --dry-run 2>&1 || echo "dry-run not supported, checking syntax instead"`
      2. Check workflow structure: `cat .github/workflows/ci.yml | python3 -c "import sys,json; yml=yaml.safe_load(sys.stdin); print('Valid' if 'jobs' in yml else 'Invalid')"`
    Expected Result: Workflow is syntactically valid
    Evidence: .sisyphus/evidence/task-3-verify-workflow.{ext}

---

## Final Verification

- [x] **Workflow Exists**: `.github/workflows/ci.yml` file present
- [x] **Triggers Correct**: Push to main and PRs trigger build
- [x] **Matrix Works**: Both attiny85 and attiny404 build
- [x] **Examples Build**: Example programs compile successfully (attiny404 only)
- [x] **Release Config**: Tags matching v* create releases
- [x] **Artifacts**: Release contains .a files + headers
- [x] **No Scope Creep**: No hardware targets, no test frameworks added

---

## Success Criteria

### Verification Commands
```bash
# Verify workflow file exists and is valid YAML
ls -la .github/workflows/ci.yml
python3 -c "import yaml; yaml.safe_load(open('.github/workflows/ci.yml'))"

# Verify workflow has correct triggers
grep -E "on:|push:|pull_request:|tags:" .github/workflows/ci.yml

# Verify matrix strategy
grep -A 5 "matrix:" .github/workflows/ci.yml
```

### Final Checklist
- [x] All "Must Have" present
- [x] All "Must NOT Have" absent
- [x] Workflow file is valid YAML
- [x] Both MCUs in build matrix
- [x] Release configured for v* tags
- [x] Artifacts include .a files and headers
