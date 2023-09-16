using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

namespace CSCompiler {
    public class Program {
        public static void Main(string[] args) {
            Console.WriteLine("Hello, World!\n--- --- ---\n");

            string fileName = "Example.cs";
            if (File.Exists(fileName)) {
                string text = File.ReadAllText(fileName);
                SyntaxTree syntax = CSharpSyntaxTree.ParseText(text);
                CSharpCompilationOptions options = new(
                    OutputKind.ConsoleApplication,
                    optimizationLevel: OptimizationLevel.Release,
                    assemblyIdentityComparer: DesktopAssemblyIdentityComparer.Default
                );

                string runtimeFolder = RuntimeEnvironment.GetRuntimeDirectory().Replace('\\', '/').TrimEnd('/');
                Console.WriteLine(runtimeFolder);

                List<MetadataReference> refs = new() {
                    MetadataReference.CreateFromFile(runtimeFolder + "/System.Runtime.dll"),
                    MetadataReference.CreateFromFile(runtimeFolder + "/System.Private.CoreLib.dll"),
                };

                //NOTE: https://stackoverflow.com/questions/62128565/net-core-system-private-corelib-dll-vs-system-runtime

                //SEE: https://github.com/dotnet/roslyn/issues/49498
                //  https://github.com/jaredpar/basic-reference-assemblies
                Assembly.GetEntryAssembly().GetReferencedAssemblies()
                    .ToList()
                    .ForEach(a => {
                        //Console.WriteLine(Assembly.Load(a).Location);
                        refs.Add(MetadataReference.CreateFromFile(Assembly.Load(a).Location));
                    });

                Console.WriteLine("Exists = " + File.Exists(runtimeFolder + "/System.Private.CoreLib.dll"));
                Assembly.LoadFile(runtimeFolder + "/System.Private.CoreLib.dll").GetReferencedAssemblies()
                    .ToList()
                    .ForEach(a => {
                        Console.WriteLine("    " + Assembly.Load(a).Location);
                    });

                CSharpCompilation compilation = CSharpCompilation.Create(
                    "ExampleProgram",
                    new SyntaxTree[] { syntax },
                    refs.ToArray(),
                    options
                );

                string exeName = "ExampleProgram.exe";
                EmitResult result = compilation.Emit(exeName);
                if (result.Success) {
                    Console.WriteLine("Compilation SUCCESS! Executable created as: " + exeName);
                    try {
                        Process.Start(exeName).WaitForExit();
                    } catch (Exception e) {
                        Console.WriteLine(e.Message);
                        Console.WriteLine("!");
                    }
                    Console.WriteLine("?");
                } else {
                    Console.WriteLine("Compilation FAILED!");
                    foreach (Diagnostic? info in result.Diagnostics.Where(d => d.Severity == DiagnosticSeverity.Error)) {
                        Console.WriteLine(info);
                    }
                }
            } else {
                Console.WriteLine(fileName + " was not found.");
            }
            
        }
    }
}
