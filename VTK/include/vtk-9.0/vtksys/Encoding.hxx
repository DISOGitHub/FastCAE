/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing#kwsys for details.  */
#ifndef vtksys_Encoding_hxx
#define vtksys_Encoding_hxx

#include <vtksys/Configure.hxx>

#include <string>
#include <vector>

namespace vtksys {
class vtksys_EXPORT Encoding
{
public:
  // Container class for argc/argv.
  class vtksys_EXPORT CommandLineArguments
  {
  public:
    // On Windows, get the program command line arguments
    // in this Encoding module's 8 bit encoding.
    // On other platforms the given argc/argv is used, and
    // to be consistent, should be the argc/argv from main().
    static CommandLineArguments Main(int argc, char const* const* argv);

    // Construct CommandLineArguments with the given
    // argc/argv.  It is assumed that the string is already
    // in the encoding used by this module.
    CommandLineArguments(int argc, char const* const* argv);

    // Construct CommandLineArguments with the given
    // argc and wide argv.  This is useful if wmain() is used.
    CommandLineArguments(int argc, wchar_t const* const* argv);
    ~CommandLineArguments();
    CommandLineArguments(const CommandLineArguments&);
    CommandLineArguments& operator=(const CommandLineArguments&);

    int argc() const;
    char const* const* argv() const;

  protected:
    std::vector<char*> argv_;
  };

  /**
   * Convert between char and wchar_t
   */

#if vtksys_STL_HAS_WSTRING

  // Convert a narrow string to a wide string.
  // On Windows, UTF-8 is assumed, and on other platforms,
  // the current locale is assumed.
  static std::wstring ToWide(const std::string& str);
  static std::wstring ToWide(const char* str);

  // Convert a wide string to a narrow string.
  // On Windows, UTF-8 is assumed, and on other platforms,
  // the current locale is assumed.
  static std::string ToNarrow(const std::wstring& str);
  static std::string ToNarrow(const wchar_t* str);

#  if defined(_WIN32)
  /**
   * Convert the path to an extended length path to avoid MAX_PATH length
   * limitations on Windows. If the input is a local path the result will be
   * prefixed with \\?\; if the input is instead a network path, the result
   * will be prefixed with \\?\UNC\. All output will also be converted to
   * absolute paths with Windows-style backslashes.
   **/
  static std::wstring ToWindowsExtendedPath(std::string const&);
  static std::wstring ToWindowsExtendedPath(const char* source);
  static std::wstring ToWindowsExtendedPath(std::wstring const& wsource);
#  endif

#endif // vtksys_STL_HAS_WSTRING

}; // class Encoding
} // namespace vtksys

#endif
