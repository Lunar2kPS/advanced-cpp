//using System.Runtime.InteropServices;

//namespace MainCSProj {
//    public class Program {
//        public static void Main(string[] args) {
//            string folder = RuntimeEnvironment.GetRuntimeDirectory().Replace('\\', '/');
//            string cscPath = Path.Combine(folder, "csc.exe").Replace('\\', '/');

//            Console.WriteLine(cscPath);
//        }
//    }
//}

using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace MainCSProjNamespace {
    public class Program {
        public static void Main(string[] args) {
            Console.WriteLine("Hi from C# :)");
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LibraryArgs {
            public IntPtr message;
            public int number;
        }

        public static int SimpleMethod(IntPtr arg, int argLength) {
            Console.WriteLine(argLength + " vs. " + Marshal.SizeOf<LibraryArgs>());
            if (argLength < Marshal.SizeOf<LibraryArgs>())
                return 1;

            Task test = Task.Run(async () => {
                int sum = 0;
                int interval = 400;
                for (int i = 0; i < 10; i++) {
                    Console.WriteLine("Waiting... (" + sum + "ms)");
                    await Task.Delay(interval);
                    sum += interval;
                }
            });
            LibraryArgs args = Marshal.PtrToStructure<LibraryArgs>(arg);
            for (int i = 0; i < 3; i++) {
                Console.WriteLine("Hello world from C#! (" + GetString(args.message) + ")");
                Thread.Sleep(1000);
            }
            return 0;
        }

        private static string GetString(IntPtr value) {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ?
                Marshal.PtrToStringUni(value) :
                Marshal.PtrToStringUTF8(value);
        }
    }
}
