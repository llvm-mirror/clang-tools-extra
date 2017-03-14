using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace LLVM.ClangTidy
{
    public class FilterInfo
    {
        [YamlAlias("Pattern")]
        public string Pattern { get; set; }

        [YamlAlias("Replacement")]
        public string Replacement { get; set; }

        [YamlAlias("Multiline")]
        public bool Multiline { get; set; } = false;
    }

    /// <summary>
    /// Reads the list of output window regex filters from Yaml
    /// </summary>
    public static class OutputFilterDatabase
    {
        static FilterInfo[] Filters_ = null;

        class FilterRoot
        {
            [YamlAlias("Filters")]
            public FilterInfo[] Filters { get; set; }
        }

        static OutputFilterDatabase()
        {
            using (StringReader Reader = new StringReader(Resources.VSOutputFilter))
            {
                Deserializer D = new Deserializer(namingConvention: new PascalCaseNamingConvention());
                var Root = D.Deserialize<FilterRoot>(Reader);
                Filters_ = Root.Filters;

                foreach (var filter in Filters_)
                {
                    if (filter.Replacement == null)
                        filter.Replacement = "";
                }
            }
        }

        public static IEnumerable<FilterInfo> Filters
        {
            get
            {
                return Filters_;
            }
        }
    }
}
