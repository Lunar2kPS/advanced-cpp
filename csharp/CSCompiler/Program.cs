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
using Microsoft.Extensions.DependencyModel;

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
                    //MetadataReference.CreateFromFile(runtimeFolder + "/System.Runtime.dll"),
                    //MetadataReference.CreateFromFile(runtimeFolder + "/System.Private.CoreLib.dll"),
                };

                //string testPath = Path.GetDirectoryName(typeof(object).Assembly.Location).Replace('\\', '/') + '/';

                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Private.CoreLib.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Runtime.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Console.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "netstandard.dll"));

                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Text.RegularExpressions.dll")); // IMPORTANT!
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Linq.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Linq.Expressions.dll")); // IMPORTANT!

                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.IO.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Net.Primitives.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Net.Http.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Private.Uri.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Reflection.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.ComponentModel.Primitives.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Globalization.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Collections.Concurrent.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "System.Collections.NonGeneric.dll"));
                //refs.Add(MetadataReference.CreateFromFile(testPath + "Microsoft.CSharp.dll"));

                //foreach (string filePath in Directory.GetFiles(runtimeFolder).Where(p => p.EndsWith(".dll")
                //&& p.Contains("System.") && !p.Contains("System.IO.Compression.Native.dll")
                //)) {
                //    refs.Add(MetadataReference.CreateFromFile(filePath));
                //}

                //NOTE: https://stackoverflow.com/questions/62128565/net-core-system-private-corelib-dll-vs-system-runtime

                //SEE: https://github.com/dotnet/roslyn/issues/49498
                //  https://github.com/jaredpar/basic-reference-assemblies
                Assembly.GetEntryAssembly().GetReferencedAssemblies()
                    .ToList()
                    .ForEach(a => {
                        Console.WriteLine(Assembly.Load(a).Location);
                        refs.Add(MetadataReference.CreateFromFile(Assembly.Load(a).Location));
                    });

                refs.AddRange(
                    //DependencyContext.Default.CompileLibraries
                    //    .SelectMany(c => c.ResolveReferencePaths())
                    //    .Select(a => MetadataReference.CreateFromFile(a))

                    //Absolutely ONLY .NET Core references!
                    DependencyContext.Default.CompileLibraries
                        .First(c => c.Name.StartsWith("Microsoft.NETCore"))
                        .ResolveReferencePaths()
                        .Select(a => MetadataReference.CreateFromFile(a))
                );

                foreach (CompilationLibrary c in DependencyContext.Default.CompileLibraries)
                    Console.WriteLine("name = " + c.Name);

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
