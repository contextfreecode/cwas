using System;

class Prompt {
    static void Main(string[] args) {
        Console.Write("hi ");
        while (true) {
            var c = Console.ReadKey(true);
            if (c.KeyChar == '\r') break;
            Console.Write((int)c.KeyChar);
        }
        Console.WriteLine();
    }
}
