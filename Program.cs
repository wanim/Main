using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace regex
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            const string str1 = @"\(\b(?<enum>IDSC_\w*),\s*""(?<name>\w*)""\)";
            const string str2 = @"\(\b(?<enum>IDSC_\w*),\s*(?<func>UI_\w+Create+\w*)\)";

            var fs = new FileStream("input.txt", FileMode.Open, FileAccess.Read,
                FileShare.ReadWrite | FileShare.Delete);
            var content = new StreamReader(fs, Encoding.UTF8).ReadToEnd();

            var rx1 = new Regex(str1, RegexOptions.None);
            var rx2 = new Regex(str2, RegexOptions.None);
            //var matches1 = rx1.Matches(content);
            //var matches2 = rx2.Matches(content);

            fs.Close();

            var screensArray = new List<ScreenInfo>();

            foreach (Match match1 in rx1.Matches(content))
            {
                foreach (Match match2 in rx2.Matches(content))
                {
                    if (match1.Result("$1") == match2.Result("$1"))
                    {
                        screensArray.Add(new ScreenInfo
                        {
                            ScreenEnum = match1.Result("$1"),
                            ScreenName = match1.Result("$2"),
                            ScreenFunc = match2.Result("$2")
                        });
                    }
                }
            }

            // output
            foreach (var screen in screensArray)
            {
                Console.WriteLine(screen.ScreenEnum + "\t\t" + screen.ScreenFunc + "\t\t" + screen.ScreenName);
            }
            Console.ReadKey();
        }

        private class ScreenInfo
        {
            public string ScreenEnum;
            public string ScreenFunc;
            public string ScreenName;
        }
    }
}