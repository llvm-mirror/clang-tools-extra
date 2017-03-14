using EnvDTE;
using Microsoft.VisualStudio.Shell;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace LLVM.ClangTidy
{
    static class Utility
    {
        public static IEnumerable<string> SplitPath(string FileOrDir)
        {
            string P = Path.GetDirectoryName(FileOrDir);
            do
            {
                yield return P;
                P = Path.GetDirectoryName(P);
            } while (P != null);
        }

        public static bool HasClangTidyFile(string Folder)
        {
            string ClangTidy = Path.Combine(Folder, ".clang-tidy");
            return File.Exists(ClangTidy);
        }

        public static bool MatchWildcardString(string Value, string Pattern)
        {
            string RE = Regex.Escape(Pattern).Replace(@"\*", ".*");
            return Regex.IsMatch(Value, RE);
        }

        public static string GetActiveSourceFileFullPath(bool searchForCppFile)
        {
            DTE dte = Package.GetGlobalService(typeof(DTE)) as DTE;

            if (dte.ActiveDocument != null)
            {
                string filePath = dte.ActiveDocument.FullName;
                if (searchForCppFile && !filePath.EndsWith(".cpp"))
                {
                    string cppFilePath = Regex.Replace(filePath, @"\..*$", ".cpp");
                    if (File.Exists(cppFilePath))
                        return cppFilePath;
                }

                return filePath;
            }
            else
                return null;
        }

        public static string GetActiveSourceFileName()
        {
            DTE dte = Package.GetGlobalService(typeof(DTE)) as DTE;

            if (dte.ActiveDocument != null)
                return dte.ActiveDocument.Name;
            else
                return null;
        }

        public static string GetActiveSourceFileHeaderName()
        {
            string file_name = GetActiveSourceFileName();

            if (!string.IsNullOrEmpty(file_name))
            {
                file_name = Regex.Replace(file_name, @"\..*$", ".h");
            }

            return file_name;
        }

        public static string GetVsixInstallPath()
        {
            string code_base = Assembly.GetExecutingAssembly().CodeBase;
            UriBuilder uri = new UriBuilder(code_base);
            string path = Uri.UnescapeDataString(uri.Path);
            return Path.GetDirectoryName(path);
        }
    }
}
