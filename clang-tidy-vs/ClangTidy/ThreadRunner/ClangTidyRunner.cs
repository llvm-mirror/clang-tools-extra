using System;
using System.ComponentModel;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using EnvDTE;

namespace LLVM.ClangTidy
{
    /// <summary>
    /// Launches clang-tidy.exe, waits for results and displays them in output window
    /// </summary>
    public static class ClangTidyRunner
    {
        private static readonly string ClangTidyExeName = "clang-tidy.exe";
        private static Guid OutputWindowGuid = new Guid(GuidList.guidClangTidyOutputWndString);
        private static readonly string OutputWindowTitle = "Clang Tidy";
        private static IVsOutputWindowPane OutputWindowPane;
        private static string ExtensionDirPath;
        private static BackgroundWorker InfoWorker;
        private static volatile bool IsUpdateInProgress = false;

        static ClangTidyRunner()
        {
            IVsOutputWindow outWindow = Package.GetGlobalService(typeof(SVsOutputWindow)) as IVsOutputWindow;
            outWindow.CreatePane(ref OutputWindowGuid, OutputWindowTitle, 1, 1);
            outWindow.GetPane(ref OutputWindowGuid, out OutputWindowPane);

            ExtensionDirPath = Utility.GetVsixInstallPath();
        }

        public static void RunClangTidyProcess()
        {
            PrepareOutputWindow();

            string activeDocumentFullPath = Utility.GetActiveSourceFileFullPath(true);

            if (activeDocumentFullPath != null)
            {
                string arguments = "-header-filter=" + Utility.GetActiveSourceFileHeaderName();
                arguments += " " + activeDocumentFullPath;

                if (!IsUpdateInProgress)
                {
                    IsUpdateInProgress = true;

                    StartBackgroundInfoWorker();

                    OutputWindowPane.OutputStringThreadSafe(">> Running " + ClangTidyExeName + " with arguments: '" + arguments + "'\n");

                    BackgroundThreadWorker worker = new BackgroundThreadWorker(ExtensionDirPath + "\\" + ClangTidyExeName, arguments);
                    worker.ThreadDone += HandleThreadFinished;

                    System.Threading.Thread workerThread = new System.Threading.Thread(worker.Run);
                    workerThread.Start();
                }
            }
            else
            {
                OutputWindowPane.OutputStringThreadSafe(">> No source file available!");
            }
        }

        private static void HandleThreadFinished(object sender, EventArgs out_args)
        {
            InfoWorker.CancelAsync();

            ValidationResultFormatter.AcquireTagsFromOutput((out_args as OutputEventArgs).Output);
            ValidationClassifier.InvalidateActiveClassifier();

            // Wait for info worker thread to finish
            while (InfoWorker.CancellationPending) { System.Threading.Thread.Sleep(50); }

            OutputWindowPane.OutputStringThreadSafe("\n");
            OutputWindowPane.OutputStringThreadSafe(ValidationResultFormatter.FormatOutputWindowMessage((out_args as OutputEventArgs).Output));
            OutputWindowPane.OutputStringThreadSafe(">> Finished");

            IsUpdateInProgress = false;
        }

        /// <summary>
        /// Background worker is a simple thread responsible for updating output window 
        /// (tell user something is happening in background) while clang-tidy thread does it's job.
        /// </summary>
        private static bool StartBackgroundInfoWorker()
        {
            if (InfoWorker != null && (InfoWorker.IsBusy || InfoWorker.CancellationPending))
            {
                throw new Exception("while trying to start new worker thread another worker thread found running!");
            }

            if (InfoWorker == null)
            {
                InfoWorker = new BackgroundWorker();
            }

            InfoWorker.WorkerReportsProgress = true;
            InfoWorker.WorkerSupportsCancellation = true;

            InfoWorker.DoWork += new DoWorkEventHandler(BackgroundWorkerDoWork);
            InfoWorker.ProgressChanged += new ProgressChangedEventHandler(BackgroundWorkerUpdateProgress);
            InfoWorker.RunWorkerAsync();

            return true;
        }

        private static void BackgroundWorkerDoWork(object sender, DoWorkEventArgs args)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            int i = 0;

            while (true)
            {
                if (worker.CancellationPending)
                {
                    break;
                }

                System.Threading.Thread.Sleep(500);
                i++;
                worker.ReportProgress(i);
            }
        }

        /// <summary>
        /// Just put comma every now and then to ensure user clang-tidy is still working
        /// </summary>
        private static void BackgroundWorkerUpdateProgress(object sender, ProgressChangedEventArgs args)
        {
            OutputWindowPane.OutputStringThreadSafe(".");
        }

        private static void PrepareOutputWindow()
        {
            OutputWindowPane.Clear();
            OutputWindowPane.Activate();

            // Force output window to front
            DTE dte = Package.GetGlobalService(typeof(SDTE)) as DTE;
            dte.ExecuteCommand("View.Output");
        }
    }
}
