using System;
using System.IO.MemoryMappedFiles;
using System.Threading;

namespace MemoryMappedFileExample {
    public static class Program {
        private const long ByteCount = 32;
        private const string SharedMemoryName = "Shared RAM Example";

        private static Mutex mutex;
        private static bool isClosed = false;
        private static void Main(string[] args) {
            try {
                mutex = new Mutex(false, "Memory Mapped File Example", out bool isFirstProgram);
                mutex.WaitOne();

                if (isFirstProgram)
                    WriteProgram();
                else
                    ReadProgram();
            } finally {
                if (mutex != null) {
                    if (!isClosed) {
                        isClosed = true;
                        mutex.ReleaseMutex();
                    }
                    mutex.Dispose();
                }
            }
        }

        private static void WriteProgram() {
            using (MemoryMappedFile memory = MemoryMappedFile.CreateNew(SharedMemoryName, ByteCount)) {
                using (MemoryMappedViewAccessor accessor = memory.CreateViewAccessor(0, ByteCount)) {
                    Console.WriteLine("Created MemoryMappedFile with " + accessor.Capacity + "bytes in capacity!");
                    for (long i = 0; i < accessor.Capacity; i++) {
                        accessor.Write(i, (byte) (i % 16));
                    }
                }

                //NOTE: Let's release early, so that we can let others (processes) read from this shared RAM!
                isClosed = true;
                mutex.ReleaseMutex();

                //NOTE: This keeps the MemoryMappedFile memory open:
                Console.WriteLine("Press ENTER to allow the OS to release the MemoryMappedFile.");
                Console.ReadLine();
            }
        }

        private static void ReadProgram() {
#pragma warning disable CA1416 //Windows-only code below!
            using (MemoryMappedFile memory = MemoryMappedFile.OpenExisting(SharedMemoryName)) {
#pragma warning restore CA1416

                using (MemoryMappedViewAccessor accessor = memory.CreateViewAccessor()) {
                    long capacity = accessor.Capacity;
                    long previewCount = ByteCount;
                    if (capacity > ByteCount) {
                        Console.WriteLine("The given memory-mapped file was rounded up in size to the next system page size (rounded up from " + ByteCount + " bytes to a capacity of " + accessor.Capacity + " bytes).");
                        Console.WriteLine("We'll print out a fiew values past the original size we used, just to demo the memory is zeroed out!");
                        previewCount = ByteCount + 10;
                        Console.WriteLine("previewCount = " + ByteCount + " + " + 10 + " = " + previewCount);
                    }
                    for (long i = 0; i < previewCount; i++) {
                        Console.WriteLine("    byte " + i + " / " + capacity + ": " + accessor.ReadByte(i));
                    }
                }
            }
        }
    }
}
