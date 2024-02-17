using System;
using System.Threading;

namespace CSharpTestProject {
    public static class Program {
        public static void Main() {
            Console.WriteLine("Waiting for the mutex...");

            Mutex m = null;
            try {
                m = new Mutex(false, "Mutex Example");
                m.WaitOne();

                Console.WriteLine("This application owns the mutex.\nPress ENTER to release the mutex and exit.");
                Console.ReadLine();
            } finally {
                if (m != null) {
                    m.ReleaseMutex();
                    m.Dispose();
                }
            }
        }
    }
}
