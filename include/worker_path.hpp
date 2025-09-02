//
// Created by Germain Jones on 01/09/2025.
//
// TODO: Reimplement AI generated code.
#ifndef DMLIB_WORKER_PATH_HPP
#define DMLIB_WORKER_PATH_HPP

#include <filesystem>
#include <string>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#endif
namespace dml::detail {
  inline std::filesystem::path executable_dir() {
#if defined(_WIN32)
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf, buf + n).parent_path();
#elif defined(__APPLE__)
    // macOS: _NSGetExecutablePath (omit for brevity); fallback to /proc/self/exe absent
    char buf[PATH_MAX];
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) == 0) return std::filesystem::path(buf).parent_path();
    return std::filesystem::current_path();
#else
    char buf[PATH_MAX];
    ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (n > 0) {
      buf[n] = '\0';
      return std::filesystem::path(buf).parent_path();
    }
    return std::filesystem::current_path();
#endif
  }

  inline std::filesystem::path this_library_dir() {
#if defined(_WIN32)
    HMODULE hm = nullptr;
    // Use the address of this function to locate the containing module
    GetModuleHandleExW(
      GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
      reinterpret_cast<LPCWSTR>(&this_library_dir),
      &hm
    );
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(hm, buf, MAX_PATH);
    return std::filesystem::path(buf, buf + n).parent_path();
#else
    Dl_info info{};
    if (dladdr(reinterpret_cast<void*>(&this_library_dir), &info) && info.dli_fname) {
      return std::filesystem::path(info.dli_fname).parent_path();
    }
    // Static link fallback → we’re inside the executable
    return executable_dir();
#endif
  }

  inline std::filesystem::path find_worker(const std::string& name) {
    const auto libdir = this_library_dir();
    const auto exedir = executable_dir();

    // Candidates in priority order
    const std::filesystem::path candidates[] = {
#if defined(_WIN32)
      libdir / ".." / "bin" / (name + ".exe"), // installed layout
      exedir / (name + ".exe"), // build tree if everything lands in bin/
      libdir / (name + ".exe") // Windows often keeps DLLs and EXEs together
#else
      libdir / ".." / "bin" / name, // installed layout
      exedir / name, // build tree bin/
      libdir / name
#endif
    };

    for (const auto& p: candidates) {
      std::error_code ec;
      if (std::filesystem::exists(p, ec))
        return std::filesystem::canonical(p, ec).empty() ? p : std::filesystem::canonical(p);
    }
    throw std::runtime_error("DMLib: worker executable '" + name + "' not found near library/executable.");
  }

  inline std::string addition_worker_path() { return find_worker("DistributedAddition").string(); }
  inline std::string subtraction_worker_path() { return find_worker("DistributedSubtraction").string(); }
  inline std::string multiplication_worker_path() { return find_worker("DistributedMultiplication").string(); }
} // namespace dml::detail

#endif // DMLIB_WORKER_PATH_HPP
