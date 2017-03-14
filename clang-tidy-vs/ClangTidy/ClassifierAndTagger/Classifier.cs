using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.Text.Classification;
using Microsoft.VisualStudio.Text.Tagging;
using Microsoft.VisualStudio.Text;

namespace LLVM.ClangTidy
{
    /// <summary>
    /// Tag classifier finds every instance of ValidationTag within a given span.
    /// </summary>
    class ValidationClassifier : IClassifier
    {
        private IClassificationType ClassificationType;
        private ITagAggregator<ValidationTag> Tagger;
        private static ValidationClassifier ActiveClassifier = null;
        private SnapshotSpan CurrentSpan;

        internal ValidationClassifier(ITagAggregator<ValidationTag> tagger, IClassificationType classificationType)
        {
            Tagger = tagger;
            ClassificationType = classificationType;
        }

        /// <summary>
        /// Get every ValidationTag instance within the given span. Generally, the span in 
        /// question is the displayed portion of the file currently open in the Editor
        /// </summary>
        /// <param name="span">The span of text that will be searched for validation tags</param>
        /// <returns>A list of every relevant tag in the given span</returns>
        public IList<ClassificationSpan> GetClassificationSpans(SnapshotSpan span)
        {
            ActiveClassifier = this;

            // After clang-tidy returns new results, tags will be automatically created only for 
            //   newly appearing text lines or focused code window.
            // To force-refresh tags in current window store single span for the whole file and 
            //   call Invalidate() on this span when clang-tidy results are ready.
            var WholeSpan = new SnapshotSpan(span.Snapshot, 0, span.Snapshot.Length);
            CurrentSpan = WholeSpan;

            IList<ClassificationSpan> classifiedSpans = new List<ClassificationSpan>();

            var tags = Tagger.GetTags(span);

            foreach (IMappingTagSpan<ValidationTag> tagSpan in tags)
            {
                var validationSpan = tagSpan.Span.GetSpans(span.Snapshot).First();
                classifiedSpans.Add(new ClassificationSpan(validationSpan, ClassificationType));
            }

            return classifiedSpans;
        }

        /// <summary>
        /// Create an event for when the Classification changes
        /// </summary>
        public event EventHandler<ClassificationChangedEventArgs> ClassificationChanged;

        /// <summary>
        /// Force refresh a span stored on last update (assume it corresponds to currently active document)
        /// </summary>
        public void Invalidate()
        {
            ClassificationChanged?.Invoke(this, new ClassificationChangedEventArgs(CurrentSpan));
        }

        static public void InvalidateActiveClassifier()
        {
            if (ActiveClassifier != null)
                ActiveClassifier.Invalidate();
        }
    }
}
