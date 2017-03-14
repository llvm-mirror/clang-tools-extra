using System.Collections.Generic;
using System.ComponentModel.Composition;
using Microsoft.VisualStudio.Language.Intellisense;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.Utilities;

namespace LLVM.ClangTidy
{
    [Export(typeof(IIntellisenseControllerProvider))]
    [Name("ClangTidy QuickInfo Controller")]
    [ContentType("text")]
    internal class QuickInfoControllerProvider : IIntellisenseControllerProvider
    {
        [Import]
        internal IQuickInfoBroker QuickInfoBroker { get; set; }

        public IIntellisenseController TryCreateIntellisenseController(ITextView textView, IList<ITextBuffer> subjectBuffers)
        {
            return new QuickInfoController(textView, subjectBuffers, this);
        }
    }
}
