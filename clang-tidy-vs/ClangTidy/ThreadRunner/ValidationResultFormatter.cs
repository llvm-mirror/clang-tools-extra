using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace LLVM.ClangTidy
{
    static public class ValidationResultFormatter
    {
        public class SingleValidationResult
        {
            public string File;
            public int Line;
            public int Column;
            public string Classification;
            public string Description;
            public string TidyCheckName;
            public string CodeLine;
            public string HighlightSymbol;
        }

        static public List<SingleValidationResult> ValidationResults { private set; get; } = new List<SingleValidationResult>();

        public static string FormatOutputWindowMessage(string message)
        {
            foreach (var filter in OutputFilterDatabase.Filters)
            {
                Regex rgx = new Regex(filter.Pattern, filter.Multiline ? RegexOptions.Multiline : RegexOptions.None);
                message = rgx.Replace(message, filter.Replacement);
            }

            return message;
        }

        public static void AcquireTagsFromOutput(string message)
        {
            ValidationResults.Clear();

            // Clang-tidy output pattern is as goes: 
            // (file name full path):(line):(column): (warning/error): (description) [clang-tidy check name]
            //     (indented code line)
            //                   (^ character pointing at warning/error in code line)
            string pattern = @"(.*):(\d+):(\d+):\s(.*):\s(.*)\s\[(.*)\]\r\n(.*)\r";

            MatchCollection matches = Regex.Matches(message, pattern);
            foreach (Match match in matches)
            {
                if (match.Groups.Count == 8)
                {
                    SingleValidationResult res = new SingleValidationResult();
                    res.File = match.Groups[1].Value;
                    res.File = res.File.Replace('/', '\\'); // use Microsoft's favorite backslashes in paths instead of standard slashes
                    int.TryParse(match.Groups[2].Value, out res.Line);
                    int.TryParse(match.Groups[3].Value, out res.Column);
                    res.Line -= 1;   // line and column number start from 1 but in MEF numbering starts with 0
                    res.Column -= 1; //
                    res.Classification = match.Groups[4].Value;
                    res.Description = match.Groups[5].Value;
                    res.TidyCheckName = match.Groups[6].Value;
                    res.CodeLine = match.Groups[7].Value;
                    // Extract symbol where warning/error is present using given column number as start and searching for ending delimiter
                    res.HighlightSymbol = res.CodeLine.Substring(res.Column);
                    res.HighlightSymbol = Regex.Match(res.HighlightSymbol, @"^([a-zA-z0-9_]+)").ToString();

                    ValidationResults.Add(res);
                }
            }
        }
    }
}
