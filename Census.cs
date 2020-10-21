using System;
using System.Text;

class CancelException: Exception {}

class Field {
    public string Label { init; get; }

    public int Max { init; get; }

    StringBuilder value = new();

    void Cancel() {
        value.Clear();
        Draw();
        Write(ConsoleColor.Red, "-");
        Console.WriteLine();
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

    protected virtual bool KeepChar(char c) {
        return true;
    }

    public string Prompt() {
        ConsoleCancelEventHandler cancelHandler = delegate { Cancel(); };
        Console.CancelKeyPress += cancelHandler; 
        try {
            do {
                Draw();
            } while (HandleChar());
            Console.WriteLine();
            return value.ToString();
        } finally {
            Console.CancelKeyPress -= cancelHandler;
        }
    }

    void Write(ConsoleColor color, string message) {
        Console.ForegroundColor = color;
        try {
            Console.Write(message);
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

class Census {
    static void Main(string[] args) {
        while (true) {
            try {
                var name = new Field {
                    Label = "Name", Max = 5,
                }.Prompt();
                var age = new ConstrainedField {
                    Label = "Age", Max = 3, Approved = "0123456789",
                }.Prompt();
                Console.WriteLine($"{name} is {age} years old");
            } catch (CancelException) {
                // Just keep going.
            }
        }
    }
}
