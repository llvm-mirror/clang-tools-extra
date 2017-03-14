using System.Collections.Generic;
using Microsoft.VisualStudio.Language.Intellisense;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Editor;

namespace LLVM.ClangTidy
{
    internal class QuickInfoController : IIntellisenseController
    {
        private ITextView TextView;
        private IList<ITextBuffer> SubjectBuffers;
        private QuickInfoControllerProvider Provider;
        private IQuickInfoSession Session;

        internal QuickInfoController(ITextView textView, IList<ITextBuffer> subjectBuffers, QuickInfoControllerProvider provider)
        {
            TextView = textView;
            SubjectBuffers = subjectBuffers;
            Provider = provider;

            TextView.MouseHover += OnTextViewMouseHover;
        }

        private void OnTextViewMouseHover(object sender, MouseHoverEventArgs e)
        {
            SnapshotPoint? point = GetMousePosition(new SnapshotPoint(TextView.TextSnapshot, e.Position));

            if (point != null)
            {
                ITrackingPoint triggerPoint = point.Value.Snapshot.CreateTrackingPoint(point.Value.Position,
                    PointTrackingMode.Positive);

                // Find the broker for this buffer
                if (!Provider.QuickInfoBroker.IsQuickInfoActive(TextView))
                {
                    Session = Provider.QuickInfoBroker.CreateQuickInfoSession(TextView, triggerPoint, true);
                    Session.Start();
                }
            }
        }

        public void Detach(ITextView textView)
        {
            if (TextView == textView)
            {
                TextView.MouseHover -= this.OnTextViewMouseHover;
                TextView = null;
            }
        }

        private SnapshotPoint? GetMousePosition(SnapshotPoint topPosition)
        {
            // Map this point down to the appropriate subject buffer.
            return TextView.BufferGraph.MapDownToFirstMatch
                (
                topPosition,
                PointTrackingMode.Positive,
                snapshot => SubjectBuffers.Contains(snapshot.TextBuffer),
                PositionAffinity.Predecessor
                );
        }

        public void ConnectSubjectBuffer(ITextBuffer subjectBuffer)
        {
        }

        public void DisconnectSubjectBuffer(ITextBuffer subjectBuffer)
        {
        }
    }
}
