using System;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SocketsExample {
    public class Program {
        private const int MaxMessageLength = 16; //NOTE: Includes NUL terminator character
        private static readonly IPEndPoint ServerEndpoint = new(IPAddress.Parse("127.0.0.1"), 8085);
        public static Encoding DefaultEncoding => Encoding.UTF8;

        public static void Main(string[] args) {
            bool isServer = args.Contains("--server");
            bool isClient = !isServer;
            bool useUDP = args.Contains("--udp");
            bool useTCP = !useUDP;

            Console.WriteLine("C# Sockets Example Program!" + (isClient ? " (CLIENT)" : "") + (isServer ? " (SERVER)" : "") + (useTCP ? " (TCP)" : "") + (useUDP ? " (UDP)" : ""));

            if (useTCP) {
                if (isServer) {
                    try {
                        using (Socket server = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)) {
                            Console.WriteLine("Successfully created the server listening socket.");
                            server.Bind(ServerEndpoint);
                            Console.WriteLine("Successfully bound the server listening socket to " + ServerEndpoint + "!");
                            server.Listen(1);
                            Console.WriteLine("Successfully began listening on the server socket!");
                            Socket client = server.Accept();
                            Console.WriteLine("Successfully accepted a client from " + client.RemoteEndPoint + "!");
                            Console.Write("[SERVER] Enter your TCP message: ");
                            string message = Console.ReadLine();
                            byte[] bytes = EncodeWithNUL(message, MaxMessageLength);
                            int messageBytesLength = Math.Min(bytes.Length, MaxMessageLength);
                            int bytesSent = client.Send(bytes, 0, messageBytesLength, SocketFlags.None);
                            Console.WriteLine("Sent " + messageBytesLength + " bytes to client at " + client.RemoteEndPoint + ".");
                            server.Close();
                            Console.WriteLine("Successfully closed the server listening socket.");
                        }
                    } catch (Exception e) {
                        Console.WriteLine("[SERVER] An exception occurred! " + e.GetType().Name + ": " + e.Message + "\n\n" + e.StackTrace + "\n");
                    }
                } else if (isClient) {
                    try {
                        using (Socket client = new(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)) {
                            Console.WriteLine("Successfully created the client socket.");
                            client.Connect(ServerEndpoint);
                            Console.WriteLine("Successfully connected to the server at 127.0.0.1:8085!");
                            Console.WriteLine("The client automatically bound to " + client.LocalEndPoint + "!");
                            byte[] bytes = new byte[MaxMessageLength];
                            int bytesReceived = client.Receive(bytes);
                            string message = DecodeWithNUL(bytes);
                            Console.WriteLine("Successfully received a message from the server at 127.0.0.1:8085 containing " + bytesReceived + " bytes!\n    " + message);
                            client.Close();
                            Console.WriteLine("Successfully closed the client socket.");
                        }
                    } catch (Exception e) {
                        Console.WriteLine("[CLIENT] An exception occurred! " + e.GetType().Name + ": " + e.Message + "\n\n" + e.StackTrace + "\n");
                    }
                }
            } else if (useUDP) {
                if (isServer) {
                    try {
                        using (Socket server = new(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp)) {
                            Console.WriteLine("Successfully created the server listening socket.");
                            server.Bind(ServerEndpoint);
                            Console.WriteLine("Successfully bound the server listening socket to " + ServerEndpoint + "!");
                            EndPoint clientEndpoint = server.LocalEndPoint;
                            byte[] bytes = new byte[MaxMessageLength];
                            int bytesReceived = server.ReceiveFrom(bytes, 0, MaxMessageLength, SocketFlags.None, ref clientEndpoint);
                            string message = DecodeWithNUL(bytes);
                            Console.WriteLine("Successfully received a message from a client at " + clientEndpoint + " containing " + bytesReceived + " bytes!\n    " + message);
                            server.Close();
                            Console.WriteLine("Successfully closed the server listening socket.");
                        }
                    } catch (Exception e) {
                        Console.WriteLine("[SERVER] An exception occurred! " + e.GetType().Name + ": " + e.Message + "\n\n" + e.StackTrace + "\n");
                    }
                } else if (isClient) {
                    try {
                        using (Socket client = new(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp)) {
                            Console.WriteLine("Successfully created the client socket.");
                            Console.Write("[CLIENT] Enter your UDP message: ");
                            string message = Console.ReadLine();
                            byte[] bytes = EncodeWithNUL(message, MaxMessageLength);
                            int messageBytesLength = Math.Min(bytes.Length, MaxMessageLength);
                            int bytesSent = client.SendTo(bytes, 0, messageBytesLength, SocketFlags.None, ServerEndpoint);
                            Console.WriteLine("The client automatically bound to " + client.LocalEndPoint + "!");
                            Console.WriteLine("Sent " + bytesSent + " bytes to server at " + ServerEndpoint + ".");

                            client.Close();
                            Console.WriteLine("Successfully closed the client socket.");
                        }
                    } catch (Exception e) {
                        Console.WriteLine("[CLIENT] An exception occurred! " + e.GetType().Name + ": " + e.Message + "\n\n" + e.StackTrace + "\n");
                    }
                }
            }
        }

        public static byte[] EncodeWithNUL(string value, int maxBytes = -1) {
            if (value == null)
                throw new ArgumentNullException(nameof(value));

            Encoding e = DefaultEncoding;
            int countWithoutNUL = e.GetByteCount(value);
            byte[] bytes = new byte[countWithoutNUL + 1];
            e.GetBytes(value, 0, value.Length, bytes, 0);

            if (maxBytes > 0 && bytes.Length > maxBytes)
                Console.WriteLine("WARNING: Name exceeds the max number of bytes (" + bytes.Length + " / " + maxBytes + "). Note that this includes the NUL terminator.");
            return bytes;
        }

        public static string DecodeWithNUL(byte[] bytes) {
            int nulIndex = -1;
            for (int i = 0; i < bytes.Length; i++) {
                if (bytes[i] == 0) {
                    nulIndex = i;
                    break;
                }
            }

            if (nulIndex < 0)
                return null;
            return DefaultEncoding.GetString(bytes, 0, nulIndex); //NOTE: System.Text.Encodings don't use the NUL terminator, so we don't include it when using their functions. Only up to, but NOT including the NUL character.
        }
    }
}
