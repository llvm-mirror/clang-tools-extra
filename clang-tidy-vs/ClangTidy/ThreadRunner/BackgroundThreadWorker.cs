using System;
using System.Diagnostics;
using System.Text;

namespace LLVM.ClangTidy
{
    internal class OutputEventArgs : EventArgs
    {
        public readonly string Output;

        public OutputEventArgs(string output)
        {
            Output = output;
        }
    }

    public class BackgroundThreadWorker
    {
        public event EventHandler ThreadDone;
        private string ExeName;
        private string Arguments;
        public BackgroundThreadWorker(String exeName, String arguments)
        {
            ExeName = exeName;
            Arguments = arguments;
        }
        public void Run()
        {
            string result = RunExternalExe(ExeName, Arguments);
            ThreadDone?.Invoke(this, new OutputEventArgs(result));
        }

        // Run External executable using background thread
        private string RunExternalExe(string filename, string arguments = null)
        {
            var process = new System.Diagnostics.Process();

            process.StartInfo.FileName = filename;
            if (!string.IsNullOrEmpty(arguments))
            {
                process.StartInfo.Arguments = arguments;
            }

            process.StartInfo.CreateNoWindow = true;
            process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            process.StartInfo.UseShellExecute = false;

            process.StartInfo.RedirectStandardError = true;
            process.StartInfo.RedirectStandardOutput = true;

            var stdOutput = new StringBuilder();
            process.OutputDataReceived += (sender, args) => stdOutput.AppendLine(args.Data);

            string stdError = null;
            try
            {
                process.Start();
                process.BeginOutputReadLine();
                stdError = process.StandardError.ReadToEnd();
                process.WaitForExit();
            }
            catch (Exception e)
            {
                throw new Exception("error while executing " + FormatErrorMsg(filename, arguments) + ": " + e.Message, e);
            }

            if (process.ExitCode == 0)
            {
                return stdOutput.ToString();
            }
            else
            {
                var message = new StringBuilder();

                if (!string.IsNullOrEmpty(stdError))
                {
                    message.AppendLine(stdError);
                }

                if (stdOutput.Length != 0)
                {
                    message.AppendLine("Std output:");
                    message.AppendLine(stdOutput.ToString());
                }

                throw new Exception(FormatErrorMsg(filename, arguments) + 
                    " finished with exit code = " + process.ExitCode + ": " + message);
            }
        }

        private string FormatErrorMsg(string filename, string arguments)
        {
            return "'" + filename +
                ((string.IsNullOrEmpty(arguments)) ? string.Empty : " " + arguments) + "'";
        }
    }
}
