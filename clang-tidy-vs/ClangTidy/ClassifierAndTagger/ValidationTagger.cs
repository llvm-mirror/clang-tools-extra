using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Tagging;

namespace LLVM.ClangTidy
{
    /// <summary>
    /// Validation tag for ITagger.
    /// </summary>
    internal class ValidationTag : ITag
    {
        public string validationMessage { get; private set; }
        internal ValidationTag(string message)
        {
            validationMessage = message;
        }
    }

    /// <summary>
    /// This class implements ITagger for ValidationTag.  It is responsible for creating
    /// ValidationTag TagSpans, which our GlyphFactory will then create glyphs for.
    /// </summary>
    internal class ValidationTagger : ITagger<ValidationTag>
    {
        ITextBuffer Buffer;

        internal ValidationTagger(ITextBuffer buffer)
        {
            Buffer = buffer;
        }

        public event EventHandler<SnapshotSpanEventArgs> TagsChanged
        {
            add { }
            remove { }
        }

        /// <summary>
        /// This method creates ValidationTag TagSpans over a set of SnapshotSpans.
        /// </summary>
        /// <param name="spans">A set of spans we want to get tags for.</param>
        /// <returns>The list of ValidationTag TagSpans.</returns>
        //IEnumerable<ITagSpan<ValidationTag>> ITagger<ValidationTag>.GetTags(NormalizedSnapshotSpanCollection spans)
        public IEnumerable<ITagSpan<ValidationTag>> GetTags(NormalizedSnapshotSpanCollection spans)
        {
            foreach (SnapshotSpan curSpan in spans)
            {
                foreach (ValidationResultFormatter.SingleValidationResult res in ValidationResultFormatter.ValidationResults)
                {
                    // Check if clang-tidy validation result is inside given span (file and line number comparison)
                    if (res.Line >= curSpan.Start.GetContainingLine().LineNumber && res.Line <= curSpan.End.GetContainingLine().LineNumber &&
                        string.Compare(res.File, curSpan.Snapshot.TextBuffer.Properties.GetProperty<ITextDocument>(typeof(ITextDocument)).FilePath, true) == 0)
                    {
                        // Check if found line contains code stored during the last validation
                        var resultLine = curSpan.Snapshot.GetLineFromLineNumber(res.Line);
                        if (resultLine.GetText().Contains(res.CodeLine))
                        {
                            var validationKeywordSpan = new SnapshotSpan(curSpan.Snapshot,
                                resultLine.Start.Position + res.Column,
                                res.HighlightSymbol.Length);

                            yield return new TagSpan<ValidationTag>(validationKeywordSpan, new ValidationTag(res.Description));
                        }
                    }
                }
            }
        }
    }
}
