using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.Language.Intellisense;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Tagging;
using System.Linq;

namespace LLVM.ClangTidy
{
    internal class QuickInfoSource : IQuickInfoSource
    {
        private ITagAggregator<ValidationTag> Aggregator;
        private ITextBuffer Buffer;
        private bool IsDisposed = false;

        public QuickInfoSource(ITextBuffer buffer, ITagAggregator<ValidationTag> aggregator)
        {
            Aggregator = aggregator;
            Buffer = buffer;
        }

        public void AugmentQuickInfoSession(IQuickInfoSession session, IList<object> quickInfoContent, out ITrackingSpan applicableToSpan)
        {
            applicableToSpan = null;

            if (IsDisposed)
                throw new ObjectDisposedException("QuickInfoSource");

            var triggerPoint = (SnapshotPoint)session.GetTriggerPoint(Buffer.CurrentSnapshot);

            if (triggerPoint == null)
                return;

            foreach (IMappingTagSpan<ValidationTag> curTag in Aggregator.GetTags(new SnapshotSpan(triggerPoint, triggerPoint)))
            {
                var tagSpan = curTag.Span.GetSpans(Buffer).First();
                applicableToSpan = Buffer.CurrentSnapshot.CreateTrackingSpan(tagSpan, SpanTrackingMode.EdgeExclusive);
                quickInfoContent.Add(curTag.Tag.validationMessage);
            }
        }

        public void Dispose()
        {
            if (!IsDisposed)
            {
                GC.SuppressFinalize(this);
                IsDisposed = true;
            }
        }
    }
}
