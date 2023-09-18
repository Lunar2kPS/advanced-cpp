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
using System.Runtime.InteropServices;

namespace MainCSProj {
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
            if (argLength < Marshal.SizeOf<LibraryArgs>())
                return 1;

            LibraryArgs args = Marshal.PtrToStructure<LibraryArgs>(arg);
            Console.WriteLine("Hello world from C#! (" + GetString(args.message) + ")");
            return 0;
        }

        private static string GetString(IntPtr value) {
            return RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ?
                Marshal.PtrToStringUni(value) :
                Marshal.PtrToStringUTF8(value);
        }
    }
}
