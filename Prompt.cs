using System;
using System.Text;
using System.Threading;

class CancelException: Exception {}

class Field {
    public string Label { init; get; }

    public int Max { init; get; }

    StringBuilder value = new();

    protected virtual bool KeepChar(char c) {
        return true;
    }

    void Cancel() {
        value.Clear();
        Draw();
        Write(ConsoleColor.Red, "-");
    }

    void Draw() {
        Console.Write($"\r{new string(' ', Console.WindowWidth)}\r");
        Write(ConsoleColor.Blue, $"{Label}: ");
        Console.Write(value);
    }

    bool HandleChar() {
        var c = Console.ReadKey(true);
        switch (c.Key) {
            case ConsoleKey.Backspace:
                if (value.Length > 0) {
                    value.Length -= 1;
                }
                break;
            case ConsoleKey.Enter:
                return value.Length == 0;
            case ConsoleKey.Escape:
                Cancel();
                throw new CancelException();
            default:
                if (value.Length < Max && KeepChar(c.KeyChar)) {
                    value.Append(c.KeyChar);
                }
                break;
        }
        return true;
    }

    public string Prompt() {
        ConsoleCancelEventHandler cancel = delegate { Cancel(); };
        Console.CancelKeyPress += cancel; 
        try {
            Draw();
            while (HandleChar()) {
                Draw();
            }
            Console.WriteLine();
            return value.ToString();
        } finally {
            Console.CancelKeyPress -= cancel;
        }
    }

    void Write(ConsoleColor color, string message) {
        Console.ForegroundColor = ConsoleColor.Red;
        try {
            Console.WriteLine("-");
        } finally {
            Console.ResetColor();
        }
    }
}

class ConstrainedField: Field {
    public string Approved { init; get; }

    protected override bool KeepChar(char c) {
        return Approved.Contains(c);
    }
}

class Program {
    static void Main(string[] args) {
        while (true) {
            try {
                var name = new Field { Label = "Name", Max = 5 }.Prompt();
                var age = Int32.Parse(new ConstrainedField {
                    Label = "Age", Approved = "0123456789", Max = 3,
                }.Prompt());
                Console.WriteLine($"{name} is {age} years old");
            } catch (CancelException) {
                // Just keep going.
            }
        }
    }
}
